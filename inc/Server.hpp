#pragma once

# include "Webserv.hpp"

/* Server es la clase que contiene la información de cada servidor
individual */

class Server
{
	public:
		uint32_t						_host; // Host o 127.0.0.1 por defecto
		uint16_t						_port;  // Puerto en el que escucha (listen)
		std::string						_server_name; // Debe agregarse a /etc/hosts para funcionar
		std::string						_root; // Carpeta raíz del directorio del sitio
		unsigned long					_client_max_body_size; // Limita el tamaño máximo del cuerpo de una solicitud HTTP
		std::string						_index; // Cuando un usuario accede a un directorio sin especificar un archivo
		std::map<short, std::string>	_error_pages; // Ejemplo: Error 404
		// std::vector<Location> 			_locations;
		// std::string					_root_location;
		// std::string					_index_location;
		// std::vector<short>			_methods_location; // GET POST DELETE
		// std::vector<std::string>	_cgi_path;
		// std::vector<std::string>	_cgi_ext;
        struct sockaddr_in 				_server_address; // Identifica la dirección y puerto del servidor.
        int     						_listen_socket; // Identifica al socket que quedará a la espera de conexiones

		Server();
		~Server();
		Server(const Server &other);
		Server &operator=(const Server &other);

	private:
	
};
