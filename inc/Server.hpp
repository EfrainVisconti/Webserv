#ifndef SERVER_HPP
#define SERVER_HPP

# include "Webserv.hpp"

class Location;

/* Server es la clase que contiene la información de cada servidor
individual */

class Server
{
	public:
		uint32_t						host; // Host o 127.0.0.1 por defecto
		uint16_t						port;  // Puerto en el que escucha (listen)
		std::string						server_name; // Debe agregarse a /etc/hosts para funcionar
		std::string						root; // Carpeta raíz del directorio del sitio
		unsigned long					client_max_body_size; // Limita el tamaño máximo del cuerpo de una solicitud HTTP
		std::string						index; // Cuando un usuario accede a un directorio sin especificar un archivo
		std::map<short, std::string>	error_pages; // Ejemplo: Error 404
		std::vector<Location> 			locations;
        struct sockaddr_in 				server_address; // Identifica la dirección y puerto del servidor.
        int     						listen_socket; // Identifica al socket que quedará a la espera de conexiones

		Server();
		~Server();
		Server(const Server &other);
		Server &operator=(const Server &other);

	private:
	
};

#endif
