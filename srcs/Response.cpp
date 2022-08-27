//
// Created by Ilnur on 22.08.2022.
//

#include "../includes/Response.hpp"
#include "../includes/Location.hpp"
#include "../includes/utils.hpp"
#include "../includes/Headers.hpp"
#include "../includes/Logger.hpp"


Response::Response(std::map<std::string, std::string> header, Server& server, Client* client) : header(header), server(server), client(client){
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

std::pair<std::string, bool>	Response::is_location(std::vector<Location>& locations, std::string check){
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it) // бежим по локешенам
        if ( ((*it).get_path() == check.substr(0, (*it).get_path().size()))
        && ((*it).get_locations().find("root") != (*it).get_locations().end())) // если пути совпали то возвращяем путь и тру
            return std::pair<std::string, bool>(check.replace(0, (*it).get_path().size(), (*it).get_locations().find("root")->second), true);
    return std::pair<std::string, bool>("", false); // иначе пустую строку и фалс
}

std::pair<std::string, bool>	Response::is_index(std::vector<Location>& locations, std::string check){
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it)
        if ( ((*it).get_path() == check.substr(0, (*it).get_path().size()))
        && ((*it).get_locations().find("root") != (*it).get_locations().end()))
            if ( (*it).get_locations().find("index") != (*it).get_locations().end() )
                return std::pair<std::string, bool>((*it).get_locations().find("index")->second, true);
    if (server.getParams().find("index") != server.getParams().end())
        return std::pair<std::string, bool>(server.getParams().find("index")->second, true);
    return std::pair<std::string, bool>("", false);
}

std::pair<std::string, bool>	Response::is_bodySize(std::vector<Location>& locations, std::string check){
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it)
        if ( ((*it).get_path() == check.substr(0, (*it).get_path().size()))
        && ((*it).get_locations().find("root") != (*it).get_locations().end()) )
            if ( (*it).get_locations().find("body_size") != (*it).get_locations().end() )
                return std::pair<std::string, bool>((*it).get_locations().find("body_size")->second, true);
    if (server.getParams().find("body_size") != server.getParams().end())
        return std::pair<std::string, bool>(server.getParams().find("body_size")->second, true);
    return std::pair<std::string, bool>("", false);
}

std::pair<std::string, bool>	Response::is_method(std::vector<Location>& locations, std::string check){
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it)
        if (((*it).get_path() == check.substr(0, (*it).get_path().size())) &&
            ((*it).get_locations().find("root") != (*it).get_locations().end()))
            if ((*it).get_locations().find("method_allowed") != (*it).get_locations().end())
                return std::pair<std::string, bool>((*it).get_locations().find("method_allowed")->second, true);
    if (server.getParams().find("method_allowed") != server.getParams().end())
        return std::pair<std::string, bool>(server.getParams().find("method_allowed")->second, true);
    return std::pair<std::string, bool>("", false);
}

std::pair<std::string, bool>    Response::is_CGI(std::vector<Location>& locations, std::string check, std::string tmpURI){
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it)
    {
        if (((*it).get_path() == check.substr(0, (*it).get_path().size()))
        && ((*it).get_locations().find("extension_cgi") != (*it).get_locations().end()))
        {
            cgi_path = (*it).get_locations().find("exec_cgi")->second;
            std::string s = (*it).get_locations().find("extension_cgi")->second;
            std::string delimiter = " ";
            size_t pos = 0;
            std::string token;

            while ((pos = s.find(delimiter)) != std::string::npos) {
                token = s.substr(0, pos);
                if (tmpURI.substr(tmpURI.size() - token.size()) == token)
                    return std::pair<std::string, bool>(token, true);
                s.erase(0, pos + delimiter.length());
            }

            if (((pos = s.find(delimiter)) == std::string::npos) && s.size() != 0)
                if (tmpURI.substr(tmpURI.size() - s.size()) == s)
                    return std::pair<std::string, bool>(s, true);
        }
    }
    return std::pair<std::string, bool>("", false);
}

bool Response::is_autoindex(std::vector<Location>& locations, std::string check){
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it)
        if ( ((*it).get_path() == check.substr(0, (*it).get_path().size()))
        && ((*it).get_locations().find("root") != (*it).get_locations().end()) )
            return (*it).get_autoindex();
    return server.getAutoIndex();
}

void    Response::build_response(void){ // собираем ответ
    Logger logger(1, "log.txt");
    std::string tmp = this->header.find("uri")->second;
    logger.logging(3, tmp);
    std::string method = "";
    std::pair<std::string, bool> check = this->is_location(this->server.getLocations(), this->header.find("uri")->second);
    std::pair<std::string, bool> method_allowed = is_method(this->server.getLocations(), this->header.find("uri")->second);
    this->autoidx = is_autoindex(this->server.getLocations(), this->header.find("uri")->second);

    logger.logging(1, "URI in header check: "+ to_string(check.second));
    if (!check.second)
        uri = tmp.replace(0, 1, this->server.getParams().find("root")->second);
    else {
        uri = check.first;
        logger.logging(1, "URI init " + check.first);
    }

    if (this->header.find("method") != header.end() )
        method = this->header.find("method")->second;
    logger.logging(1, "method: " + method);
    logger.logging(1, "uri: " + this->uri);
    if (method_allowed.second && (method_allowed.first.find(method) == std::string::npos))
    {
        std::string body = "";
        Headers rsp_header;
        body = errorPage("405", "Method not Allowed", this->error);
        rsp_header.headersHTTP("405 Method Not Allowed", body.size(), uri, 0);
        this->response = rsp_header.getHeaderHTTP();
        this->response += body;
    }
    else if (method == "HEAD")
        this->method_head();
    else if (method == "GET")
        this->method_get();
    else if (method == "POST")
        this->method_post();
    else if (method == "PUT")
        this->method_put();
}

void    Response::method_head(){
    struct stat buffer;
    int status = 0;
    std::size_t check = 0;

    if ((check = uri.find('?')) != std::string::npos)
        uri.erase(check, uri.size() - check);

    status = lstat(uri.c_str(), &buffer);
    if (status == -1){
        Headers rsp_header;
        rsp_header.headersHead("404 Not Found");
        this->response = rsp_header.getHeaderHTTP();
    }
    else if ((buffer.st_mode & S_IFMT) == S_IFREG){
        Headers rsp_header;
        rsp_header.headersHead("200 OK");
        this->response = rsp_header.getHeaderHTTP();
    }
    else if ((buffer.st_mode & S_IFMT) == S_IFDIR){
        std::pair<std::string, bool> check = this->is_index(this->server.getLocations(), this->header.find("uri")->second);

        if ( check.second == true ){
            uri += "/" + check.first;
            if ( !(status = lstat(uri.c_str(), &buffer)) ){
                Headers rsp_header;
                rsp_header.headersHead("200 OK");
                this->response = rsp_header.getHeaderHTTP();
            }
            else{
                Headers rsp_header;
                rsp_header.headersHead("404 Not Found");
                this->response = rsp_header.getHeaderHTTP();
            }
        }
        else{
            Headers rsp_header;
            rsp_header.headersHead("200 OK");
            this->response = rsp_header.getHeaderHTTP();
        }
    }
}

void		Response::method_get(){
    Logger logger(1, "log.txt");
    struct stat buffer;
    int status = 0;
    std::string body = "";
    char buf[32000] = {0};

    std::size_t pos = 0;
    std::string tmpURI;
    if ((pos = uri.find('?')) != std::string::npos)
        tmpURI = uri.substr(0, pos);
    else
        tmpURI = uri;
    std::pair<std::string, bool> checkCGI = this->is_CGI(this->server.getLocations(), this->header.find("uri")->second, tmpURI);
    if (checkCGI.second){
        Headers env;
        env.headersCGI(header, client, server, uri);
        CGI cgi(cgi_path, tmpURI, env.getHeaderCGI());
        this->response = cgi.getStatus();
        this->response += cgi.getOutput();
        return ;
    }
    status = lstat(uri.c_str(), &buffer);
    logger.logging(1, "Read " + uri);
    if (status == -1){
        logger.logging(3, "[GET] Read uri status: " + to_string(status));
        body = errorPage("404", "Page Not Found", this->error);
        logger.logging(1, "body 404: " + body);
        Headers rsp_header;
        rsp_header.headersHTTP("404 Not Found", body.size(), uri, 0);
        this->response = rsp_header.getHeaderHTTP();
        this->response += body;
    }
    else if ((buffer.st_mode & S_IFMT) == S_IFREG){
        int fd = open(uri.c_str(), O_RDONLY);
        int bytes = 0;

        Headers rsp_header;
        rsp_header.headersHTTP("200 OK", buffer.st_size, uri, TIMESPEC);
        this->response = rsp_header.getHeaderHTTP();
        while ((bytes = read(fd, buf, 32000)) > 0) {
            buf[bytes] = 0;
            body += buf;
        }
        close(fd);
        this->response += body;
        if (fd == -1 || bytes == -1){
            logger.logging(2, "page not found, fd=" + to_string(fd) + " bytes=" + to_string(bytes));
            body = errorPage("404", "Page Not Found", this->error);
            Headers rsp_header;
            rsp_header.headersHTTP("404 Not Found", body.size(), uri, 0);
            this->response = rsp_header.getHeaderHTTP();
            this->response += body;
        }
    }
    else if ((buffer.st_mode & S_IFMT) == S_IFDIR){
        std::pair<std::string, bool> check = this->is_index(this->server.getLocations(), this->header.find("uri")->second);

        if (!check.second ){
            uri += "/" + check.first;
            if ( !(status = lstat(uri.c_str(), &buffer)) ){
                int fd = open(uri.c_str(), O_RDONLY);
                int bytes = 0;

                Headers rsp_header;
                rsp_header.headersHTTP("200 OK", buffer.st_size, uri, TIMESPEC);
                this->response = rsp_header.getHeaderHTTP();
                while ((bytes = read(fd, buf, 32000)) > 0) {
                    buf[bytes] = 0;
                    body += buf;
                }
                close(fd);
                this->response += body;
                if (fd == -1 || bytes == -1){
                    logger.logging(2, "page not found, fd=" + to_string(fd) + " bytes=" + to_string(bytes));
                    body = errorPage("404", "Page Not Found", this->error);
                    Headers rsp_header;
                    rsp_header.headersHTTP("404 Not Found", body.size(), uri, 0);
                    this->response = rsp_header.getHeaderHTTP();
                    this->response += body;
                }
            }
            else{
                logger.logging(2, "page not found, check.second false");
                body = errorPage("404", "Page Not Found", this->error);
                Headers rsp_header;
                rsp_header.headersHTTP("404 Not Found", body.size(), uri, 0);
                this->response = rsp_header.getHeaderHTTP();
                this->response += body;
            }
        }
        else{
            if (autoidx){
                body = autoindexGenerator(uri, this->header.find("uri")->second);
                Headers rsp_header;
                rsp_header.headersHTTP("200 OK", body.size(), uri, 0);
                this->response = rsp_header.getHeaderHTTP();
                this->response += body;
            }
            else{
                body = errorPage("403", "Forbidden", this->error);
                Headers rsp_header;
                rsp_header.headersHTTP("403 Forbidden", body.size(), uri, 0);
                this->response = rsp_header.getHeaderHTTP();
                this->response += body;
            }
        }
    }
}

void		Response::method_post()
{
    std::size_t pos = 0;
    std::string tmpURI;
    std::string body = "";

    if ((pos = uri.find('?')) != std::string::npos)
        tmpURI = uri.substr(0, pos);
    else
        tmpURI = uri;
    std::pair<std::string, bool> checkCGI = this->is_CGI(this->server.getLocations(), this->header.find("uri")->second, tmpURI);
    Headers env;
    env.headersCGI(this->header, this->client, this->server, this->uri);

    std::pair<std::string, bool> check = this->is_bodySize(this->server.getLocations(), this->header.find("uri")->second);
    if (check.second && stoull(env.getCGIbody_size()) > stoull(check.first))
    {
        body = errorPage("413", "Request Entity Too Large", this->error);
        Headers rsp_header;
        rsp_header.headersHTTP("413 Request Entity Too Large", body.size(), uri, 0);
        this->response = rsp_header.getHeaderHTTP();
        this->response += body;
        return ;
    }
    CGI cgi(this->cgi_path, this->uri, env.getHeaderCGI());
    this->response = cgi.getStatus();
    this->response += cgi.getOutput();
}

void		Response::method_put()
{
    int fd = 0;
    std::string body = "";
    bool existed = false;


    if ((fd = open(uri.c_str(), O_RDONLY)) != -1)
        existed = true;
    close(fd);
    if (this->header.find("content-lenght") != header.end())
    {
        std::pair<std::string, bool> check = this->is_bodySize(this->server.getLocations(), this->header.find("uri")->second);
        if (check.second && stoull(this->header.find("content-lenght")->second) > stoull(check.first))
        {
            body = errorPage("413", "Request Entity Too Large", this->error);
            Headers rsp_header;
            rsp_header.headersHTTP("413 Request Entity Too Large", body.size(), uri, 0);
            this->response = rsp_header.getHeaderHTTP();
            this->response += body;
            return ;
        }
    }
    if ( (fd = open(uri.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0655)) > 2 )
    {

        Headers rsp_header;
        rsp_header.headersHTTP((!existed) ? "201 Created" : "200 OK", body.size(), uri, 0);
        this->response = rsp_header.getHeaderHTTP();

        write(fd, this->header.find("body")->second.c_str(), this->header.find("body")->second.size());
    }
    else
    {
        body = errorPage("404", "Page Not Found", this->error);
        Headers rsp_header;
        rsp_header.headersHTTP("404 Not Found", body.size(), uri, 0);
        this->response = rsp_header.getHeaderHTTP();
        this->response += body;
    }
    close(fd);
}