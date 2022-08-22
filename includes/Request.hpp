//
// Created by Ilnur on 22.08.2022.
//

#pragma once
#include "main.hpp"

class Request {
private:
    std::string request;
    std::map<std::string, std::string> header;
public:
    Request(std::string request);
    ~Request();
    std::map<std::string, std::string>  getHeader(void);

};

