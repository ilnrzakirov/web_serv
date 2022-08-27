#include "../includes/Logger.hpp"
#include "../includes/main.hpp"
#include "../includes/utils.hpp"
#include "../includes/Handler.hpp"
Config *p;

void		closeFDs(int){
    Logger logger(1, "log.txt");
    std::vector<Server>::iterator it = p->getConfig()->begin();
    std::vector<Server>::iterator ite = p->getConfig()->end();
    for ( ; it != ite; ++it)
        close(it->getFd());
    std::cout << "\b\b";
    logger.logging(1, "All servers are closed");
    unlink("cgi.txt");
    unlink("out.txt");
    exit(0);
}

int main(int argc, char **argv){
    Logger logger(1, "log.txt");
    logger.logging(1, "Start program");
    if(argc != 2){
        logger.logging(4, "Bad arguments");
        return (1);
    }
    Config conf(argv[1]);
    p = &conf;
    signal(SIGINT, &closeFDs);
    signal(SIGTERM, &closeFDs);
    try {
        logger.logging(1, "Start pars config file");
        conf.parse();
    } catch(const std::exception& e) {
        logger.logging(4, e.what());
        std::cerr << e.what() << std::endl;
    }
    logger.logging(1, "Init handler");
    Handler handler(conf.getConfig());
    handler.init();
    logger.logging(1, "Start server");
    handler.run_server();
}