#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include "Webserv.hpp"

/* ServerManager es la clase principal del programa, se encarga de ejecutar
los servidores extraidos del archivo de configuración. También establece
las conexiones con clientes y la gestión de peticiones/respuestas */

class ServerManager
{
	public:                 
		ServerManager();
		~ServerManager();

		void	LaunchServers();

	private:
		std::vector<Server> _servers;

		bool	CreateSockets();
};

#endif