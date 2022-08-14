#include "../includes/Logger.hpp"
#include "../includes/main.hpp"
//#include "config_parser.cpp"
int main(int args, char **argv){
    Logger logger(1, "log.txt");
    int fd_server;
    struct sockaddr_in address;

    logger.logging(1, "Start program");
    if(args != 2){
        logger.logging(4, "Bad arguments");
        return (1);
    }

//    config_parser(argv[1]);
    logger.logging(1, "create server socket");
    if ((fd_server = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        logger.logging(4, "Socket error");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    logger.logging(1, "bind server socket");
    if (bind(fd_server, (struct sockaddr *)&address, sizeof(address))< 0){
        logger.logging(4, "Error when assigning an address");
        exit(EXIT_FAILURE);
    }
    logger.logging(1, "listen socket");
    if (listen(fd_server, 20) < 0){
        logger.logging(4, "Listen error");
        exit(EXIT_FAILURE);
    }
    int address_len = sizeof(address);
    int fd;
    logger.logging(1, "Run loop");
    while (true){
        std::cout << "WAITING\n";
        if ((fd = accept(fd_server, (struct sockaddr *)&address, (socklen_t*)&address_len)) < 0){
            logger.logging(4, "Accept failure");
            exit(EXIT_FAILURE);
        }
        logger.logging(1, "read request");
        char buf[2048] = {0};

        long count = read(fd, buf, 2048);
        std::cout << buf << std::endl;
        close(fd);
    }
    return 0;
}