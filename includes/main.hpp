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
#include <list>

#define PORT 8080

namespace std

class Request {
public:
    string method;
    string hostname;
    int port;
    string body;
};
