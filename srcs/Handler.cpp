//
// Created by Ilnur on 13.08.2022.
//

#include "../includes/Handler.hpp"
#include "../includes/Logger.hpp"


Handler::Handler(std::vector<Server>* servers) {
    this->servers = servers;
}

Handler::~Handler() {}

void Handler::init() {
    FD_ZERO(&this->write_fds);
    FD_ZERO(&this->reed_fds);
    FD_ZERO(&this->copy_read_fds);
    FD_ZERO(&this->copy_write_fds);
    for (size_t i=0; i < servers->size(); i++){
//        Здесь инициализация сервера
        (*servers)[i].init();
        FD_SET((*servers)[i].fd, &this->reed_fds);
    }
}

void Handler::run_server() {
    Logger logger(1, "log.txt");
    char    buf[2048];
    int     bytes;
    struct  timval tv;

    while(true){
        this->copy_read_fds = this->reed_fds;
        this->copy_write_fds = this->write_fds;
        select(FD_SETSIZE, &copy_read_fds, &copy_write_fds, 0, 0);
//        Нужны классы клиентов и сервера
        for (std::size_t i = 0; i < this->servers->size(); ++i)
        {
            if (FD_ISSET((*servers)[i].getFd(), &this->copy_read_fds))
            {
                Client* client = new Client((*servers)[i].getFd()); // создаем пару для сервера
                client->acceptClient(); // берем fdшник если постучались на слушающий сокет
                FD_SET(client->getFD(), &this->reed_fds); // клиента добавляем в сет fd для чтения
                this->clients.push_back(client); // добавляем в вектор клиентов
                server_count = i; // количество серверов
            }
        }
        for (std::vector<Client*>::iterator it(this->clients.begin()); it != this->clients.end(); ++it){
            if (FD_ISSET((*it)->getFD(), &this->copy_read_fds)){ //проверяем существует ли fd клиента в нашем векторе
                if ((bytes = recv((*it)->getFD(), buf, 2048, 0)) > 0 ){ // читаем с сокета (если что то прочли то: )
                    std::string fd_string = std::to_string((*it)->getFD());
                    std::string read_byte = std::to_string(bytes);
                    logger.logging(1, ("read fd" + fd_string + " total: " + read_byte + "bytes"))
                    buf[bytes] = 0; // ставим конец строки
                    (*it)->request += buf; // добавляем то что прочли к нашему реквесту
                    memset(buf, 0, 2048); // повторно заполняем нулями
                } else if (bytes == 0) { // если ничего не прочли с сокета то:
                    FD_CLR((*it)->getFD(), &this->reed_fds);
                    FD_CLR((*it)->getFD(), &this->write_fds);
                    delete *it;
                    clients.erase(it);
                    logger.logging(3, "Client disconnected")
                    break;
                }
            }
        }
    }
}