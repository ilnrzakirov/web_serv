//
// Created by Ilnur on 22.08.2022.
//

#include "../includes/Request.hpp"


Request::Request(std::string request) {
    std::size_t separtor = request.find(" "); // находим первый сепаратор (пробел)
    this->header.insert(std::pair<std::string, std::string>("method",request.substr(0, separtor))); // берем метод и записываем в мап
    std::size_t next_separator = request.find(" ", separtor); // берем второй сепаратор
    header.insert(std::pair<std::string, std::string>("uri", request.substr(separtor + 1, next_separator - separtor - 1))); // записываем индентификатор

    if ((separtor = request.find("Content-Length")) != std::string::npos) // находим длину контента
    {
        separtor += 15;
        if ((next_separator = request.find("\r\n", separtor)) != std::string::npos)
            value = request.substr(separtor, next_separator - separtor);
        header.insert(std::pair<std::string, std::string>("content-lenght", value)); // вносим длину контента в мап
    }
    separtor = request.find("\r\n\r\n");
    if ( request.size() != (separtor + 4) )
    {
        separtor += 4;
        value = request.substr(separtor, request.size() - separtor);
        header.insert(std::pair<std::string, std::string>("body", value)); // записываем тело запроса
    }
    if ((separtor = request.find("X-Secret-Header-For-Test")) != std::string::npos)
    {
        separtor += 26;
        value = "";
        if ((next_separator = request.find("\r\n", separtor)) != std::string::npos)
            value = request.substr(separtor, next_separator - separtor);
        header.insert(std::pair<std::string, std::string>("secret", value)); // запсиываем в мап x-secret
    }
}

Request::~Request() {}

std::map <std::string, std::string> Request::getHeader() {
    return this->header;
}