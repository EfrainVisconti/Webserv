#include "../inc/Webserv.hpp"
#include "../inc/ServerManager.hpp"

int main(int argc, char **argv) {

    try {
		std::string		    config;
        std::vector<Server> servers;
		ConfigParser	    cluster(servers);

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
