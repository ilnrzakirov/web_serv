//
// Created by Ilnur on 13.08.2022.
//

#include "../includes/Logger.hpp"
#include <time.h>


Logger::~Logger() {}

Logger::Logger(int level, std::string filename) {
    this->level = level;
    this->filename = filename;
    this->LogLevelNames[0] = "INFO";
    this->LogLevelNames[1] = "DEBUG";
    this->LogLevelNames[2] = "WARNING";
    this->LogLevelNames[3] = "ERROR";

}

void Logger::logging(int level, std::string msg) {
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime ( &rawtime );
    std::string date_time = asctime(timeinfo);
    std::replace(date_time.begin(), date_time.end(), '\n', ' ');
    if (level >= this->level) {
        if (level > 2) {
            std::cerr << date_time << " - " << this->LogLevelNames[level - 1] << ": " << msg << std::endl;
        }
        std::ofstream log;
        log.open(filename, ios_base::app);
        log << date_time << " - " << this->LogLevelNames[level - 1] << ": " << msg << std::endl;
        log.close();
    }
}