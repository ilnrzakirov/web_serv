//
// Created by Ilnur on 13.08.2022.
//

#pragma once
#include "main.hpp"

class Logger {
private:
    int level;
    std::string filename;
    std::string LogLevelNames[4];
    Logger();
public:
    Logger(int level, std::string filename);
    ~Logger();
    void    logging(int level, std::string msg);
};

