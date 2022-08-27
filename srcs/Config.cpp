//
// Created by Ilnur on 27.08.2022.
//

#include "../includes/Config.hpp"
#include "../includes/utils.hpp"

Config::Config(const char* filePath)
{
    std::string words[11] = {"listen", "location", "server_name",
                             "index", "body_size", "error_page", "method",
                             "root", "autoindex", "exec_cgi", "extension_cgi"};
    for (int i = 0; i < 11; ++i)
        keywords[i] = words[i];
    table.reserve(2);
    int fd = 0;
    char buf[2024] = {0};
    if ( (fd = open(filePath, O_RDONLY)) == -1 )
        throw runtime_error("Config File not found");
    while (read(fd, buf, 2024))
        content += buf;
    close(fd);
    std::size_t start = 0;
    std::size_t end = 0;
    while ( (start = content.find('#')) != std::string::npos )
    {
        end = content.find('\n', start);
        content.erase(start, end - start);
    }
}

Config::~Config() {}

std::vector<Server>* Config::getConfig(void){
    return &table;
}

void	Config::parse(void){
    std::pair<std::string, bool> ret;
    bool inServer = false, inLocation = false;
    int pos = 0;
    int	nServ = 0;
    if (std::count(this->content.begin(), this->content.end(), '{') - std::count(this->content.begin(), this->content.end(), '}') != 0)
        throw runtime_error("Config file error");
    while (this->content[pos]){
        if (this->content[pos] == '}')
            ++pos;
        while (isspace(this->content[pos]))
            ++pos;
        if (!(this->content[pos]))
            break ;
        if (this->content.substr(pos, 6) == "server")
            pos += 6;
        else
            throw runtime_error("No 'server' in cinfig file");

        while (isspace(this->content[pos]))
            ++pos;

        if (this->content[pos] == '{' ) {
            ++pos; inServer = true;
        }
        else
            throw runtime_error("Config file error");
        while (isspace(this->content[pos]))
            ++pos;
        ret = iskeywords(this->content.substr(pos), this->keywords, 11);
        Server x;
        this->table.push_back(x);
        while (ret.second == true && ret.first != "server" && inLocation == false && inServer == true){
            std::pair<std::string, std::string> param = split_param(this->content.substr(pos));
            this->table[nServ].getParams().insert(param);
            while (this->content[pos] != '\n')
                ++pos;
            while (isspace(this->content[pos]))
                ++pos;
            ret = iskeywords(this->content.substr(pos), this->keywords, 11);
            if (ret.first == "location" && inServer == true){
                while (ret.first == "location"){
                    inLocation = true;
                    pos += 8;
                    while (isspace(this->content[pos]))
                        ++pos;
                    Location loc;
                    loc.get_path() = getPath(this->content, pos);
                    pos += loc.get_path().size();
                    while (isspace(this->content[pos]) || this->content[pos] == '{')
                        ++pos;
                    ret = iskeywords(this->content.substr(pos), this->keywords, 11);
                    while (ret.second == true){
                        param = split_param(this->content.substr(pos));
                        loc.get_locations().insert(param);
                        while (this->content[pos] != '\n')
                            ++pos;
                        while (isspace(this->content[pos]))
                            ++pos;
                        ret = iskeywords(this->content.substr(pos), this->keywords, 11);
                    }
                    this->table[nServ].getLocations().push_back(loc);
                    while (inLocation && (isspace(this->content[pos]) || this->content[pos] == '}')){
                        if (this->content[pos] == '}')
                            inLocation = false;
                        ++pos;
                    }
                    while (isspace(this->content[pos]))
                        ++pos;
                    ret = iskeywords(this->content.substr(pos), this->keywords, 11);
                }
            }
        }
        nServ++;
    }
    this->check();
}


void	Config::check(void){
    for (std::size_t j = 0; j < this->table.size() - 1; ++j){
        std::map<std::string, std::string>::iterator itEnd = this->table[j].getParams().end();
        std::map<std::string, std::string>::iterator it = this->table[j].getParams().find("listen");
        for (std::size_t i = j + 1; i < this->table.size(); ++i){
            std::map<std::string, std::string>::iterator itEnd_next = this->table[i].getParams().end();
            std::map<std::string, std::string>::iterator it_next = this->table[i].getParams().find("listen");
            if ((it == itEnd) || (it_next == itEnd_next) || (*it).second == (*it_next).second)
                throw runtime_error("Port error");
        }
    }
    for (std::size_t j = 0; j < this->table.size(); ++j){
        std::map<std::string, std::string>::iterator it = this->table[j].getParams().find("autoindex");
        std::map<std::string, std::string>::iterator itEnd = this->table[j].getParams().end();
        if (it != itEnd)
            if ((*it).second == "on")
                this->table[j].getAutoIndex() = true;
        for (std::size_t i = 0; i < this->table[j].getLocations().size(); ++i){
            std::map<std::string, std::string>::iterator itL = this->table[j].getLocations()[i].get_locations().find("autoindex");
            std::map<std::string, std::string>::iterator itEndL = this->table[j].getLocations()[i].get_locations().end();
            if (itL != itEndL){
                if ((*itL).second == "on")
                    table[j].getLocations()[i].get_autoindex() = true;
            }
            else{
                if (table[j].getAutoIndex() == true)
                    table[j].getLocations()[i].get_autoindex() = true;
            }
        }
    }
}
