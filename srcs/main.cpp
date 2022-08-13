//
// Created by Toad Miranda on 8/12/22.
//

#include "../includes/main.hpp"
#include "config_parser.cpp"
int main(int args, char **argv){
    int fd_server;
    struct sockaddr_in address;

    if(args != 2){
        std::cerr << "Bad arguments\n";
        return (1);
    }

    config_parser(argv[1]);

    if ((fd_server = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        std::cerr << "Socket error\n";
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(fd_server, (struct sockaddr *)&address, sizeof(address))< 0){
        std::cerr << "Error when assigning an address\n";
        exit(EXIT_FAILURE);
    }
    if (listen(fd_server, 20) < 0){
        std::cerr << "Listen error\n";
        exit(EXIT_FAILURE);
    }
    int address_len = sizeof(address);
    int fd;
    while (true){
        std::cout << "WAITING\n";
        if ((fd = accept(fd_server, (struct sockaddr *)&address, (socklen_t*)&address_len)) < 0){
            std::cerr << "Accept failure\n";
            exit(EXIT_FAILURE);
        }
        char buf[2048] = {0};

        long count = read(fd, buf, 2048);
        std::cout << buf << std::endl;
        close(fd);
    }
    return 0;
}