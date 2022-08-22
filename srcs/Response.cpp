//
// Created by Ilnur on 22.08.2022.
//

#include "../includes/Response.hpp"
#include "../includes/Location.hpp"


Response::Response(std::map<std::string, std::string> header, Server& server, Client* client) : header(header), server(server), client(client)
{
    error_page = "";
    if (server.getParams().find("error_page") != server.getParams().end())
        error_page = server.getParams().find("error_page")->second;
    this->build_response();
}

Response::~Response() {}


std::string Response::getResponse(void) {
    return this->response;
}

// нужен конфиг

