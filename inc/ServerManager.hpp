#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include "Webserv.hpp"

class Request;
class Server;
class Response;

/* ServerManager es la clase principal del programa, se encarga de ejecutar
los servidores extraidos del archivo de configuración. También establece
las conexiones con clientes y la gestión de peticiones/respuestas */

class ServerManager
{
	public:
		ServerManager();
		~ServerManager();

		void	SetServers(std::vector<Server> &servers);
		void	LaunchServers();

		class ErrorException : public std::exception
		{
			private:
				std::string	_message;

			public:
				ErrorException(std::string str) throw()
				{
					_message = "webserv: " + str;
				}

				char const *what() const throw()
				{
					return (_message.c_str());
				}
				virtual ~ErrorException() throw() {}
		};

	private:
		std::vector<Server> 		_servers;
		std::vector<pollfd> 		_poll_fds; // Estructuras pollfd que van a ser monitoreadas
		std::map<int, Server*>		_server_map;  // Mapea fd de socket de escucha a su Server
		std::map<int, Server*>		_client_map; // Mapea fd de socket conectado a su Server
		std::map<int, Response*>	_response_map; // Mapea fd de socket conectado a su Response

		void	CreateSockets();
		void	SetSockaddr_in(Server *server);
		void	AcceptConnection(int server_fd, Server *server);
		void	HandleRequest(int connected_fd);
		void	CloseConnection(int fd);
		void	ResponseManager(int client_fd, Request &req);
		void	HandleResponse(int client_fd);
};

std::string	errorResponse(int val);

#endif