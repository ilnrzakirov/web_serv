//
// Created by Toad Miranda on 8/12/22.
//
#include <iostream>
#include <fstream>
#include <list>
std::string trimLeft(std::string str) {
    size_t startpos = str.find_first_not_of(" ");
    if(0 != startpos)
        str = str.substr(startpos);
    return str;
}
void config_parser(string fileName) {
    fstream myFile;
    std::string line;
    myFile.open(fileName, ios::in);
    if (!(myFile.is_open())) {
        std::cout << "File not created!\n";
    }
    else {
        std::list<std::string> directives = {"server", "location"};
        std::stack<std::string> stackOfFunctions;
        std::list<std::string> variables;
        while (getline(myFile, line))
        {
            line = trimLeft(line);
            std::cout << line << std::endl;
//            for(int i = 0; i < 2; i++) {
//            }
        }
    }
    myFile.close();
}