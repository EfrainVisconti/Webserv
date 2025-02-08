<<<<<<< HEAD
#include "../inc/Webserv.hpp"
=======
#include "../inc/ServerManager.hpp"
>>>>>>> refs/remotes/origin/adrian

ServerManager::ServerManager() {}


ServerManager::~ServerManager()
{
	// Cerrar todos los sockets
}


/* 
	Cierra la conexión del socket identificado por fd.
	Se cierra el socket, se elimina la estructura pollfd vinculada
	del vector _poll_fds y la posición vinculada
	del map<fd, Server> _client_map.
*/
void ServerManager::CloseConnection(int fd)
{
    if (close(fd) == -1)
		return;

	_client_map.erase(fd);

	for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end();)
	{
		if (it->fd == fd)
		{
			it = _poll_fds.erase(it);
			break;
		}
		++it;
	}
}


/* 
	Se llama a recv() para recibir los datos por el socket conectado
	identificado con client_fd.
	Se llama al metodo  parseRequest() de Request para almacenar y
	procesar la petición.

	* bytes_read: cantidad de bytes leídos.
	Si bytes_read es menor o igual a 0, se llama a CloseConnection().

	NOTA: lanza ErrorException() explícita en caso de error.
*/
void ServerManager::HandleRequest(int client_fd)
{
    char buffer[2000];
    memset(buffer, 0, 2000);
    ssize_t bytes_read = recv(client_fd, buffer, 2000, 0);

    if (bytes_read <= 0)
	{
		// Gestionar código de error HTTP (errno == EAGAIN || errno == EWOULDBLOCK)
		CloseConnection(client_fd);
	}
	// Se cambiara cuando se implemente la clase Request y Response
	else
	{
		for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end(); ++it)
		{
			if (it->fd == client_fd)
			{
				it->events = POLLOUT;
				break;
			}
		}
    }

	if (DEBUG_MODE)
	{
		std::cout << GREEN << "Request receive:\n"
				  << buffer << RESET << std::endl;
	}
}


/*
	Se llama a accept(), la cual extrae la primera solicitud de conexión en
	la cola de conexiones para el socket de escucha identificado con server_fd.
	* client_addr: contiene la dirección del cliente.
	* connected_socket: identifica al socket conectado.

	Si accept() es exitoso, se añade el nuevo socket a _poll_fds y _client_map.
	Se configura la estructura pollfd connected_pollfd para ser añadida a
	* _poll_fds: vector de estructuras a monitorear por poll().

	Se mapea el nuevo socket a su Server en
	* _client_map: map <fd socket conectado, Server asociado>.

	* fcntl() configura el socket en modo no bloqueante (O_NONBLOCK).

	NOTA: lanza ErrorException() explícita en caso de error.
*/
void	ServerManager::AcceptConnection(int server_fd, Server *server)
{
    struct sockaddr_in	client_addr;
    socklen_t client_len = sizeof(client_addr);
	int connected_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
	if (connected_socket == -1)
		throw ErrorException("accept() error. Stopping execution...");

	int flags = fcntl(connected_socket, F_GETFL, 0);
	if (flags == -1 || fcntl(connected_socket, F_SETFL, flags | O_NONBLOCK) == -1) 
	{
		close(connected_socket);
		throw ErrorException("fcntl() error. Stopping execution...");
	}

    pollfd connected_pollfd = {connected_socket, POLLIN, 0};
    _poll_fds.push_back(connected_pollfd);
	_client_map[connected_socket] = server; // NOTA: Esta por revisar si es necesario

	if (DEBUG_MODE)
	{
		std::cout << GREEN << "Connection accepted by "
				  << server->server_name << " CLIENT:"
				  << inet_ntoa(client_addr.sin_addr) << RESET << std::endl;
	}
}


/*
	Configura la estructura sockaddr_in, en la cual se identifica la
	familia de la dirección, la dirección IPv4 y el puerto (del socket),
	almacenada en server->server_address). 
*/
void	ServerManager::SetSockaddr_in(Server *server)
{
	memset(&(server->server_address), 0, sizeof(server->server_address));
	server->server_address.sin_family = AF_INET;
    //server->server_address.sin_addr.s_addr = htonl(server->_host); Forma correcta
    server->server_address.sin_addr.s_addr = server->host;
    server->server_address.sin_port = htons(server->port);
}


/*
	Crea un socket por cada servidor en std::vector<Server>.
	Implementa la secuencia socket(), bind(), listen().
	* listen_socket: identifica al socket que quedará a la espera de conexiones.
	* server_address: identifica la dirección y puerto del servidor.
	
	Se configura la estructura pollfd listen_pollfd para ser añadida a
	* _poll_fds: vector de estructuras a monitorear por poll().

	Cada socket exitosamente creado es mapeado a su Server en
	* _server_map: map <fd socket de escucha, Server asociado>.

	* setsockopt() se usa para evitar el error "Address already in use".
	* fcntl() configura el socket en modo no bloqueante (O_NONBLOCK).

	NOTA: lanza ErrorException() explícita en caso de error.
*/
void	ServerManager::CreateSockets()
{
	int opt = 1;
	int	count = 0;
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
	{
		if ((it->listen_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			throw ErrorException("socket() error. Stopping execution...");

		if (setsockopt(it->listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		{
			close(it->listen_socket);
			throw ErrorException("setsockopt() error. Stopping execution...");
		}

		int flags = fcntl(it->listen_socket, F_GETFL, 0);
		if (flags == -1 || fcntl(it->listen_socket, F_SETFL, flags | O_NONBLOCK) == -1) 
		if (fcntl(it->listen_socket, F_SETFL, O_NONBLOCK) == -1)
		{
			close(it->listen_socket);
			throw ErrorException("fcntl() error. Stopping execution...");
        }

		SetSockaddr_in(&(*it));
		if (bind(it->listen_socket, (struct sockaddr *) &(it->server_address), sizeof(it->server_address)) == -1)
		{
			close(it->listen_socket);
			throw ErrorException("bind() error. Stopping execution...");
		}

		if (listen(it->listen_socket, MAX_CONN) == -1)
		{
			close(it->listen_socket);
			throw ErrorException("listen() error. Stopping execution...");

        }

		pollfd listen_pollfd = {it->listen_socket, POLLIN, 0};
		_poll_fds.push_back(listen_pollfd);
		_server_map[it->listen_socket] = &(*it);

		if (DEBUG_MODE)
		{
			std::cout << GREEN << "Server created[" << ++count << "]"
					  << " NAME:" << it->server_name
			          << " HOST:" << it->host << " PORT:" << it->port << RESET <<std::endl;
		}
    }
}


/*
	Se inicia el bucle principal de los servidores que monitorea los sockets
	de escucha creados en la llamada a CreateSockets() y los nuevos
	sockets conectados creados con la llamada a accept(). Gestiona las
	peticiones, respuestas y cierre de "conexiones".

	Cuando poll() detecta eventos:
	* Si hay una nueva conexión, llama AcceptConnection().
	* Si un cliente tiene datos, llama HandleRequest().
	* Si hay respuesta por enviar, llama HandleResponse().
	* Si el cliente cerró la conexión o hay un error, llama CloseConnection().

	NOTA: lanza ErrorException() explícita en caso de error.
*/
void	ServerManager::LaunchServers()
{
	CreateSockets();

	int	poll_ret = 0;
	while (true)
	{
		poll_ret = poll(_poll_fds.data(), _poll_fds.size(), TIMEOUT);
		if (poll_ret == -1)
			throw ErrorException("poll() error. Stopping execution...");
		if (poll_ret == 0)
			throw ErrorException("poll() time-out. Stopping execution...");

        for (size_t i = 0; i < _poll_fds.size(); i++)
		{
			if (_poll_fds[i].fd <= 0)
				continue;

			if (_poll_fds[i].revents & POLLIN)
			{
				std::map<int, Server*>::iterator it = _server_map.find(_poll_fds[i].fd);
				if (it != _server_map.end())
                    AcceptConnection(_poll_fds[i].fd, it->second);
				else
				{
					int	mbs = 2420985; // max body size
					std::string host = "127.0.0.1";
					Request	req(mbs);
					HandleRequest(_poll_fds[i].fd, req, mbs, host);
				}
            }
            else if (_poll_fds[i].revents & POLLOUT)
			{
>>>>>>> refs/remotes/origin/adrian
				// class Rsponse;
				std::string response = defaultResponse();
				send(_poll_fds[i].fd, response.c_str(), response.size(), 0);
				std::cout << "Sending response..." << std::endl;
				CloseConnection(_poll_fds[i].fd);
			}
            else if (_poll_fds[i].revents & (POLLHUP | POLLERR))
				CloseConnection(_poll_fds[i].fd);
    	}
>>>>>>> refs/remotes/origin/adrian
	}
}

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

int parseRequest(std::string _request, int mbs, std::string host, Request &req) {
    req.parseSetup(_request, req); // Cambié para pasar el objeto correctamente
    // printRequestClass(req);
    /*if (req.getBodySize() > req.getMaxBodySize()) { // check de maxbodysize.
        req.setErrorType(405); // error de bodysize
        req.clean();
        return (req.getErrorType());
    }
    if (req.verifyMethodHost(host) == 0) { // check de metodo y host.
        req.setErrorType(500); // error de host/metodo.
        req.clean();
        return (req.getErrorType());
    }
    if (access(req.getPath().c_str(), R_OK) == -1) { // check de ruta
        req.setErrorType(404); // error de ruta
        req.clean();
        return (req.getErrorType());
    }
    if (req.getMethod() == "POST" && req.getRequestFormat().empty()) { // si el metodo es post tiene que tener formato
        req.setErrorType(406); // error de formato de peticion.
        req.clean();
        return (req.getErrorType());
    }
    if (req.getUserAgent().empty()) { // userAgent vacio
        req.setErrorType(406); // error de formato de peticion.
        req.clean();
        return (req.getErrorType());
    }*/
    return (1);
}

void ServerManager::HandleRequest(int client_fd, Request &req, int mbs, std::string host)
{
    char buffer[2000];
    memset(buffer, 0, 2000);
    ssize_t bytes_read = recv(client_fd, buffer, 2000, 0);
	std::string bufferstr = buffer;
	std::cout << bufferstr << std::endl;
	int status = parseRequest(bufferstr, mbs, host, req);
	if (status == 404){
		std::string error_response = errorResponse(404);
		send(client_fd, error_response.c_str(), error_response.size(), 0);
		CloseConnection(client_fd);
	}
	if (status == 405){
		std::string error_response = errorResponse(405);
		send(client_fd, error_response.c_str(), error_response.size(), 0);
		CloseConnection(client_fd);
	}
	if (status == 406){
		std::string error_response = errorResponse(406);
		send(client_fd, error_response.c_str(), error_response.size(), 0);
		CloseConnection(client_fd);
	}
	if (status == 500){
		std::string error_response = errorResponse(500);
		send(client_fd, error_response.c_str(), error_response.size(), 0);
		CloseConnection(client_fd);
	}
	if (bytes_read <= 0 || status == 0){
		CloseConnection(client_fd);
	}
	else{
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
