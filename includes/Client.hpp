//
// Created by Ilnur on 15.08.2022.
//

#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP
#include "main.hpp"


class Client {
private:
    int fd;
    int server_fd;
    struct sockaddr_in	client;
    struct timeval		tv;
    std::string         ip;
    std::string     response;
public:
    std::string     request;
    Client(int server_fd);
    ~Client();
    int getFD();
    void acceptClient();
    std::string &getResponse();
    int				getTime();
    std::string     getIP();
};


#endif //WEBSERV_CLIENT_HPP
