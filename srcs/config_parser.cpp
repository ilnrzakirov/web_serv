//
// Created by Toad Miranda on 8/12/22.
//
#include <iostream>
#include <fstream>
using namespace std;
void config_parser(string fileName) {
    fstream myFile;
    string line;
    myFile.open(fileName, ios::in);
    if (!(myFile.is_open())) {
        cout << "File not created!\n";
    }
    else {
        while (getline(myFile, line))
        {
            std::cout << line << std::endl;
        }
    }
    myFile.close();
}