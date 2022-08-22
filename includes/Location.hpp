//
// Created by Toad Miranda on 8/13/22.
//
#pragma once
#include "main.hpp"


class Location {
private:
    std::string path;
    bool autoidx;
    std::map<std::string, std::string> locations;
public:
    Locations();
    ~Locations();
    std::map<std::string, std::string>&	get_locations(void);
    std::string& get_path(void);
    bool& get_autoindex(void);
};

