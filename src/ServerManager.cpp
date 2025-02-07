#include "../inc/ServerManager.hpp"
#include "../inc/Request.hpp"

ServerManager::ServerManager() {}

ServerManager::~ServerManager() {}

void	ServerManager::SetServers(std::vector<Server> servers)
{
	_servers = servers;
}

/*
	Configura la estructura sockaddr_in, en la cual se identifica la
	familia de la dirección, la dirección IPv4 y el puerto (del socket). 
*/
void	ServerManager::SetSockaddr_in(Server *server)
{
	memset(&(server->_server_address), 0, sizeof(server->_server_address));
	server->_server_address.sin_family = AF_INET;
    //server->_server_address.sin_addr.s_addr = htonl(server->_host); Forma correcta
    server->_server_address.sin_addr.s_addr = server->_host;
    server->_server_address.sin_port = htons(server->_port);
}

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
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
	{
		if ((it->_listen_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			std::cerr << RED << "webserv: socket() error. Stopping execution..." << RESET << std::endl;
			return false;
		}

		if (setsockopt(it->_listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		{
			std::cerr << RED << "webserv: setsockopt() error. Stopping execution..." << RESET << std::endl;
			close(it->_listen_socket);
			return false;
		}

		if (fcntl(it->_listen_socket, F_SETFL, O_NONBLOCK) == -1)
		{
			std::cerr << RED << "webserv: fcntl() error. Stopping execution..." << RESET << std::endl;
			close(it->_listen_socket);
			return false;
        }

		SetSockaddr_in(&(*it));
		if (bind(it->_listen_socket, (struct sockaddr *) &(it->_server_address), sizeof(it->_server_address)) == -1)
		{
			std::cerr << RED << "webserv: bind() error. Stopping execution..." << RESET << std::endl;
			close(it->_listen_socket);
			return false;
		}

		if (listen(it->_listen_socket, MAX_CONN) == -1)
		{
			std::cerr << RED << "webserv: listen() error. Stopping execution..." << RESET << std::endl;
			close(it->_listen_socket);
			return false;
        }

		pollfd listen_pollfd = {it->_listen_socket, POLLIN, 0};
		_poll_fds.push_back(listen_pollfd);
		_server_map[it->_listen_socket] = &(*it);

		if (DEBUG_MODE)
		{
			std::cout << GREEN << "Server created: " << " NAME: " << it->_server_name
			          << " PORT: " << it->_port << RESET <<std::endl;
		}
    }
	return true;
}

/*

*/
void	ServerManager::LaunchServers()
{
	if (CreateSockets() == false)
		throw ErrorException("Creating sockets failed.");
	
	int	poll_ret = 0;
	int	server_status = true;
	while (server_status)
	{
		poll_ret = poll(_poll_fds.data(), _poll_fds.size(), TIMEOUT);
		if (poll_ret == -1)
		{
			std::cerr << RED << "webserv: poll() error. Stopping execution..." << RESET << std::endl;
			throw ErrorException("");
		}
		if (poll_ret == 0)
		{
			std::cerr << RED << "webserv: poll() time-out. Stopping execution..." << RESET << std::endl;
			throw ErrorException("");
		}

        for (size_t i = 0; i < _poll_fds.size(); i++)
		{
			if (_poll_fds[i].revents & POLLIN)
			{
				if (_server_map.find(_poll_fds[i].fd) != _server_map.end())
				{
                    if (AcceptConnection(_poll_fds[i].fd) == false)
					{
						server_status = false;
						break;
					}
				}
				else
				{
					int	mbs = 2420985; // max body size
					int host = INADDR_ANY;
					Request	req(mbs);
					HandleRequest(_poll_fds[i].fd, req, mbs, host);
				}
            } 
            else if (_poll_fds[i].revents & POLLOUT)
			{ 
				// class Rsponse;
				std::string response = "Hola mundo!";
				send(_poll_fds[i].fd, response.c_str(), response.size(), 0);
				std::cout << "Sending response..." << std::endl;
				CloseConnection(_poll_fds[i].fd);
			}
            else if (_poll_fds[i].revents & (POLLHUP | POLLERR))
			{
				CloseConnection(_poll_fds[i].fd);
			}
        
    	}
	}
}

/*

*/
bool	ServerManager::AcceptConnection(int server_fd)
{
    struct sockaddr_in	client_addr;
    socklen_t client_len = sizeof(client_addr);
	int connected_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
	if (connected_socket == -1)
	{
        std::cerr << RED << "webserv: accept() error found" << RESET << std::endl;
		return true;
	}

	if (fcntl(connected_socket, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << RED << "webserv: fcntl() error. Stopping execution..." << RESET << std::endl;
		close(connected_socket);
		return false;
	}

    pollfd connected_pollfd = {connected_socket, POLLIN, 0};
    _poll_fds.push_back(connected_pollfd);
	return true;
}

void ServerManager::HandleRequest(int client_fd, Request req, int mbs, int host)
{
    char buffer[2000];
    memset(buffer, 0, 2000);
    ssize_t bytes_read = recv(client_fd, buffer, 2000, 0);
	std::string bufferstr = buffer;
	int status = req.parseRequest(bufferstr, mbs, host);
	if (bytes_read <= 0 || status == 0)
		CloseConnection(client_fd);
	else
	{
        for (pollfd &pfd : _poll_fds)
		{
			if (pfd.fd == client_fd)
			{
				pfd.events = POLLOUT;
				break;
            }
        }
    }
}

void ServerManager::CloseConnection(int fd)
{
    close(fd);

    for (size_t i = 0; i < _poll_fds.size(); i++)
	{
        if (_poll_fds[i].fd == fd)
		{
            _poll_fds.erase(_poll_fds.begin() + i);
            break;
        }
    }
}