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
//        (*servers)[i].init();
        FD_SET((*servers)[i].fd, &this->reed_fds);
    }
}