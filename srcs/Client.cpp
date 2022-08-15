//
// Created by Ilnur on 15.08.2022.
//

#include "../includes/Client.hpp"

Client::Client(int server_fd) {
    this->fd = -1;
    this->server_fd = server_fd;
    this->response = "";
    memset(&client, 0, sizeof(client));
    memset(&tv, 0, sizeof(tv));
    gettimeofday(&tv, 0);
    this->request = "";
}

Client::~Client() {
    close(this->fd);
}

int Client::getFD() {
    return this->fd;
}

void Client::acceptClient() {
    int		socklen = sizeof(sockaddr_in);

    fd = accept(serverFD, (struct sockaddr*)&client, (socklen_t*)&socklen);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    ip = inet_ntoa(client.sin_addr);
}

int Client::getTime() {
    return tv.tv_sec;
}

std::string Client::getIP() {
    return this->ip;
}

std::string &Client::getResponse() {
    return this->response;
}