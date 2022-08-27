//
// Created by Ilnur on 27.08.2022.
//

#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include "main.hpp"
#if __APPLE__
#define TIMESPEC buffer.st_ctimespec.tv_sec
#endif

std::string	errorPage(std::string numCode, std::string description, std::string error_page);
std::string	autoindexGenerator(std::string path, std::string request);
std::pair<std::string, bool>	iskeywords(std::string src, std::string keywords[], int num);
std::pair<std::string, std::string>	split_param(std::string src);
std::string		getPath(std::string src, int pos);
void	trim(std::string &str);
void		closeFDs(int);


#endif //WEBSERV_UTILS_HPP
