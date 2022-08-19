//
// Created by Ilnur on 13.08.2022.
//

#ifndef WEBSERV_HANDLER_HPP
#define WEBSERV_HANDLER_HPP
#include "main.hpp"
#include "Client.hpp"
#include "Server.hpp"

class Server;
class Client;

class Handler {
private:
    std::vector<Server>*    servers;
    std::vector<Client*>    clients;
    fd_set                  reed_fds;
    fd_set                  write_fds;
    fd_set                  copy_read_fds;
    fd_set                  copy_write_fds;
    Handler();
    Handler(Handler const &handler);
    Handler &operator=(Handler const &handler);
public:
    Handler(std::vector<Server>* servers);
    ~Handler();
    void init(void);
    void run_server(void);
};


#endif //WEBSERV_HANDLER_HPP
