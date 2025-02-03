#include "../inc/ServerManager.hpp"

ServerManager::ServerManager() {}

ServerManager::~ServerManager() {}


/*
	Crea un socket por cada servidor en std::vector<Server>.
	Implementa la secuencia socket(), bind(), listen().
	* _listen_socket: identifica al socket que quedará a la espera de conexiones.
	* _server_address: identifica la dirección y puerto del servidor.

	* setsockopt() se usa para evitar el error "Address already in use".
	* fcntl() configura el socket en modo no bloqueante (O_NONBLOCK). 
*/

bool	ServerManager::CreateSockets()
{
	int opt = 1;
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
	{
		if ((it->_listen_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			std::cerr << RED << "webserv: socket() error. Stopping execution..." << RESET << std::endl;
			return false;
		}

		setsockopt(it->_listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

		it->_server_address.sin_family = AF_INET;
		it->_server_address.sin_addr.s_addr = INADDR_ANY;
    	it->_server_address.sin_port = htons(it->_port);

		if (bind(it->_listen_socket, (struct sockaddr *) &(it->_server_address), sizeof(it->_server_address)) == -1)
		{
			std::cerr << RED << "webserv: bind() error. Stopping execution..." << RESET << std::endl;
			return false;
		}

		if (listen(it->_listen_socket, MAX_CONN) == -1)
		{
			std::cerr << RED << "webserv: listen() error. Stopping execution..." << RESET << std::endl;
			return false;
        }

		if (fcntl(it->_listen_socket, F_SETFL, O_NONBLOCK) == -1)
		{
			std::cerr << RED << "webserv: fcntl() error. Stopping execution..." << RESET << std::endl;
			return false;
        }
    }
}

void	ServerManager::LaunchServers()
{
	
}