#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <sys/time.h>
#include <sys/types.h>
#include <cstdlib>
#include <dirent.h>
#include <arpa/inet.h>
#include <ctime>
#include <fcntl.h>
//#include "../srcs/config_parser.cpp"

#define PORT 8080

using namespace std;

class Request {
public:
    string method;
    string hostname;
    int port;
    string body;
};
