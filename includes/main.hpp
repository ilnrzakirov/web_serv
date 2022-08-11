#pragma once

#include <iostream>
#include <vector
#include <map>
#include <string>
#include <netinet/in.h>


typedef struct	s_server {
    std::string								name;
    std::string								host;
    int										port;
    std::string 							logFile;
    std::map<int, std::string>				errorPages;
    std::multimap<std::string, Location>	locations;
}	t_server;