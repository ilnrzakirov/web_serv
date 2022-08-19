//
// Created by Toad Miranda on 8/13/22.
//

#include "../includes/Server.hpp"

Server::Server() {
    this->fd = -1;
    this->autoIndex = false;
}

Server::~Server() {}

bool &Server::getAutoIndex() {
    return this->autoIndex;
}

int &Server::getFd() {
    return this->fd;
}

std::vector<Location> &Server::getLocations() {
    return this->locations;
}

std::map<std::string, std::string> &Server::getParams() {
    return this->params;
}

void Server::init() {
    int port = std::stoi(((*(params.find("listen"))).second)); //если параметры будут записывать в мап
    this->addr.sin_port = htons(port);
    this->addr.sin_family = AF_INET;
    this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &this->reuse, sizeof(int));
    bind(fd, (struct sockaddr*) &(this->addr), sizeof(this->addr));
    listen(fd, 2000);
}