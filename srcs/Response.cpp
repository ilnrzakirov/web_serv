//
// Created by Ilnur on 22.08.2022.
//

#include "../includes/Response.hpp"
#include "../includes/Location.hpp"


Response::Response(std::map<std::string, std::string> header, Server& server, Client* client) : header(header), server(server), client(client)
{
    this->error = "";
    if (server.getParams().find("error_page") != server.getParams().end())
        this->error = server.getParams().find("error_page")->second;
    this->build_response();
}

Response::~Response() {}


std::string Response::get_response(void) {
    return this->response;
}

// нужен конфиг

