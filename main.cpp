#include "includes/main.hpp"

int main(int args, char **argv){
    std::vector<t_server> servers;

    if(args != 2){
        std::cout << "Bad arguments\n";
        return (1);
    }
    parser(argv[1], servers);

}