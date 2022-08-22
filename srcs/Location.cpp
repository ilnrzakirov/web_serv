//
// Created by Toad Miranda on 8/13/22.
//

#include "../includes/Location.hpp"

Location::Location(): path(), autoidx(false), locations() {}

Location::~Location() {}

std::map <std::string, std::string> &Location::get_locations() {
    return this->locations;
}

bool &Location::get_autoindex() {
    return this->autoidx;
}

std::string &Location::get_path() {
    return this->path;
}