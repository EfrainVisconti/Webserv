#include "../inc/Webserv.hpp"
#include "../inc/ServerManager.hpp"

// int main(int argc, char *argv)
// {
//     std::string filename = "default.conf";
//     if (argc == 2)
//         filename = argv[1];
//     else if (argc > 2){
//         std::cout << "USAGE ./webserv opt.('filename'.conf)." << std::endl;
//         return (0);
//     }
//     int attempt = 0;
//     while (1){
//         try {
//             std::cout << "\nReading file: " << filename << ".\n";
//             // Clase de parse
//             // funciones de parse
//             // server
//             return (1);
//         }
//         catch (const std::exception &e){
//             attempt++;
//             std::cerr << e.what() << std::endl;
//             if (attempt == 1 && filename != "default.conf")
//                 filename = "default.conf";
//             else
//                 break ;
//         }
//     }
//     return 0;
// }

#include <vector>
#include <iostream>

int main() {
    try {
        Server server1;
        server1.host = INADDR_ANY;
        server1.port = 8080;
        server1.server_name = "ServerOne";
        server1.root = "/html";
        server1.client_max_body_size = 1048576;
        server1.index = "index.html";

        Server server2;
        server2.host = INADDR_ANY;
        server2.port = 8081;
        server2.server_name = "ServerTwo";
        server2.root = "/html";
        server2.client_max_body_size = 2097152;
        server2.index = "index.html";

        Server server3;
        server3.host = INADDR_ANY;
        server3.port = 9090;
        server3.server_name = "ServerThree";
        server3.root = "/html";
        server3.client_max_body_size = 5242880;
        server3.index = "index.html";

        std::vector<Server> servers;
        servers.push_back(server1);
        servers.push_back(server2);
        servers.push_back(server3);
        ServerManager serverManager;
        
        serverManager.SetServers(servers);
        serverManager.LaunchServers();
    } catch (const std::exception &e) {
        std::cerr << RED << e.what() << RESET << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
