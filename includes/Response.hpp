//
// Created by Ilnur on 22.08.2022.
//

#pragma once
#include "main.hpp"
#include "Server.hpp"
#include "Client.hpp"



class Response {
private:
    std::string response;
    std::map<std::string, std::string>	header;
    Server server;
    Client *client;
    std::string uri;
    bool autoidx;
    std::string cgi_path;
    std::string error;
public:
    Response(std::map<std::string, std::string> header, Server& server, Client* client);
    ~Response();
    void							build_response(void);
    std::pair<std::string, bool>	is_location(std::vector<Locations>& locations, std::string check);
    std::pair<std::string, bool>	is_index(std::vector<Locations>& locations, std::string check);
    bool							is_autoindex(std::vector<Locations>& locations, std::string check);
    std::pair<std::string, bool>	is_bodySize(std::vector<Locations>& locations, std::string check);
    std::pair<std::string, bool>	is_method(std::vector<Locations>& locations, std::string check);
    std::pair<std::string, bool>    is_CGI(std::vector<Locations>& locations, std::string check, std::string tmpURI);
    std::string						get_response(void);
    void							method_head();
    void							method_get();
    void							method_post();
    void							method_put();
};



