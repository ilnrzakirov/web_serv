//
// Created by Ilnur on 13.08.2022.
//

#include "../includes/Logger.hpp"


Logger::~Logger() {}

Logger::Logger(int level, std::string filename) {
    this->level = level;
    this->filename = filename;
}

void Logger::logging(int level, std::string msg) {
    if (level >= this->level) {
        std::cerr << msg << std::endl;
    }
    std::ofstream log;
    log.open(filename, ios_base::app);
    log << msg << std::endl;
}