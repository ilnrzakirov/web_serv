//
// Created by Ilnur on 13.08.2022.
//

#include "../includes/Handler.hpp"


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
            }
        }
    }
}