//
// Created by Toad Miranda on 8/13/22.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP
#include "Location.hpp"

class Server {
private:
    int fd;
    bool autoIndex; // переменная для автоиндекса (парсится с конфига)
    std::map<std::string, std::string> params;  // заполняется во время парсинга{'port', '8080'}
    std::vector<Location> locations; // заполняются во время парсинга (нужно написать класс Location)
public:
    Server();
    ~Server();
    bool &getAutoIndex();
    std::map<std::string, std::string>  &getParams(void);
    std::vector<Locations>  &getLocations(void);
    int &getFd(void);
    void    init(void); // Функция инициализации сервера (где инициализируется слушающий сокет)


};


#endif //WEBSERV_SERVER_HPP
