//
// Created by Ilnur on 27.08.2022.
//

#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP

#include "main.hpp"
#include "Server.hpp"

class Server;


class Config {
private:
    std::string			content;
    std::vector<Server>	table;
    std::string keywords[11];

public:
    Config(const char* filePath);
    ~Config(void);

    void					parse(void);
    void					check(void);
    std::vector<Server>*	getConfig(void);
};


#endif //WEBSERV_CONFIG_HPP
