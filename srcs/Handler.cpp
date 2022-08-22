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
        FD_SET((*servers)[i].getFd(), &this->reed_fds);
    }
}

void Handler::run_server() {
    Logger logger(1, "log.txt");
    char    buf[2048];
    int     bytes, server_count;
    struct  timeval tv;
    bool    is_browser;

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
                    logger.logging(1, ("read fd" + fd_string + " total: " + read_byte + "bytes"));
                    buf[bytes] = 0; // ставим конец строки
                    (*it)->request += buf; // добавляем то что прочли к нашему реквесту
                    memset(buf, 0, 2048); // повторно заполняем нулями
                } else if (bytes == 0) { // если ничего не прочли с сокета то:
                    FD_CLR((*it)->getFD(), &this->reed_fds); // удаляем дискритор из списка дискрипторов чтения
                    FD_CLR((*it)->getFD(), &this->write_fds); // удаляем дискриптори из списка записи
                    delete *it; // удаляем клиента
                    this->clients.erase(it); // удаляем клиента с вектора
                    logger.logging(3, "Client disconnected");
                    break;
                }
                std::size_t s = 0;
                is_browser = false;
                if ((s = (*it)->request.find("\r\n\r\n")) != std::string::npos){ // ищем в реквесте конец
                    std::size_t body = 0;
                    if ((body = (*it)->request.find("Content-Length")) != std::string::npos) // ищем в рекввесте длину контента
                    {
                        is_browser = true;
                        body = strtoul((*it)->request.substr(body + 15, (*it)->request.find("\r\n", body) - body).c_str(), 0, 0);

                    }
                    if (((*it)->request.substr(0, 5).find("PUT") != std::string::npos ||
                         (*it)->request.substr(0, 5).find("POST") != std::string::npos) ) // проверяем что запросы не пост и не гет
                    {
                        if ((is_browser && ((*it)->request.substr(s + 4).size() >= body)) || ((*it)->request.substr(s + 4).find("\r\n\r\n") != std::string::npos))
                        {
                            FD_SET((*it)->getFD(), &this->write_fds); // удаляем дискриторы
                            FD_CLR((*it)->getFD(), &this->reed_fds);
                        }
                        else
                            break;
                    }
                    else {
                        FD_SET((*it)->getFD(), &this->write_fds);
                        FD_CLR((*it)->getFD(), &this->reed_fds);
                    }
                }
                else
                    break ;
            }
        }
    }
}