//
// Created by Ilnur on 13.08.2022.
//

#include "../includes/Handler.hpp"
#include "../includes/Logger.hpp"
#include "../includes/Request.hpp"
#include "../includes/Response.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

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
    char    *buf = (char*)malloc(100000001);;
    int     bytes, server_count;
    struct  timeval tv;

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
                bool is_browser = false;
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
                Request request((*it)->request); // инициализация рекваста (передаем то что получили с сокета)
                logger.logging(1, "Received request from " + std::to_string((*it)->getFD()));
                logger.logging(1, "request: " + (*it)->request);
                Response response(request.getHeader(), (*servers)[server_count], *it); // инициализация респонса (передаем мап хедеров)
                (*it)->getResponse() = response.get_response(); // внутри класса респонс пока что ничего не реализовано
                (*it)->request.clear(); // очищяем реквест
            }

            if (FD_ISSET((*it)->getFD(), &this->copy_write_fds)){
                int ret = send((*it)->getFD(), (*it)->getResponse().c_str(), (*it)->getResponse().size(), 0); // отсылаем респонс
                logger.logging(1, "response text: " + (*it)->getResponse());
                if (ret <= 0){ //удаляем дискриптор с векторов чтения и записи в случаи если отослать не смогли
                    FD_CLR((*it)->getFD(), &this->reed_fds);
                    FD_CLR((*it)->getFD(), &this->write_fds);
                    delete *it;
                    clients.erase(it);
                    logger.logging(4, "Client disconnected, write fail");
                    break;
                }
                if ((unsigned long)ret < (*it)->getResponse().length()) {// если отправили меньше чем длина респонса, то респонс срезаем
                    (*it)->getResponse() = (*it)->getResponse().substr(ret);
                    logger.logging(1,  (*it)->getResponse());
                }
                else{
                    FD_CLR((*it)->getFD(), &this->write_fds); // если отправили длину респонса то
                    (*it)->getResponse().clear(); // очищяем респонс и удаляем клиента
                    delete *it;
                    clients.erase(it);
                    logger.logging(2, "Client disconnected {send completed}");
                    break;
                }
            }
            memset(&tv, 0, sizeof(tv)); // стираем запись времени
            gettimeofday(&tv, 0); // заново берем время
            if ((FD_ISSET((*it)->getFD(), &this->copy_read_fds)) && (*it)->getTime() - tv.tv_sec > 10) // time out
            {
                FD_CLR((*it)->getFD(), &this->reed_fds); // удаляем клиента с массива дискриптеров для чтения
                FD_CLR((*it)->getFD(), &this->write_fds); // удаляем клиента с массива дискриптеров для записи
                delete *it; // удаляем клиента
                clients.erase(it); // удаляем клиента с вектора клиентов
                logger.logging(2, "Client disconnected {time}");
                break;
            }
        }
    }
}