//
// Created by Toad Miranda on 8/13/22.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP
#include "Location.hpp"

class Server {
    int fd;
    int port;
    string serverName;
    string errorPage;
    string root;
    bool autoIndex;
    vector<string> index;
    vector<Location> locations;

};


#endif //WEBSERV_SERVER_HPP
