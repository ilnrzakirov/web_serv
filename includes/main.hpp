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

#define PORT 8080

using namespace std;

class Request {
public:
    string method;
    string hostname;
    int port;
    string body;
};
