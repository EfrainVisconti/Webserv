#include "../inc/Webserv.hpp"
#include "../inc/ServerManager.hpp"

int main(int argc, char *argv)
{
    std::string filename = "default.conf";
    if (argc == 2)
        filename = argv[1];
    else if (argc > 2){
        std::cout << "USAGE ./webserv opt.('filename'.conf)." << std::endl;
        return (0);
    }
    int attempt = 0;
    while (1){
        try {
            std::cout << "\nReading file: " << filename << ".\n";
            // Clase de parse
            // funciones de parse
            // server
            return (1);
        }
        catch (const std::exception &e){
            attempt++;
            std::cerr << e.what() << std::endl;
            if (attempt == 1 && filename != "default.conf")
                filename = "default.conf";
            else
                break ;
        }
    }
    return 0;
}