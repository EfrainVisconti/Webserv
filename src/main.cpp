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

// /**
//  * @brief Signal handler for SIGPIPE signal.
//  * 
//  * This function handles the SIGPIPE signal. It is a placeholder for handling the signal when a broken pipe occurs.
//  * 
//  * @param sig The signal number (usually SIGPIPE).
//  */
// void sigpipeHandle(int sig) 
// {
// 	if(sig)
// 	 {

// 	 }
// }

int main(int argc, char **argv) {

    try {
		std::string		    config;
        std::vector<Server> servers;  
		ConfigParser	    cluster(servers); 

		//signal(SIGPIPE, sigpipeHandle); 
		config = (argc == 1 ? "configs/default.conf" : argv[1]);
		cluster.createCluster(config);
        cluster.print();

        ServerManager serverManager(cluster.getServers());
        serverManager.LaunchServers();
    } catch (const std::exception &e) {
        std::cerr << RED << e.what() << RESET << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
