#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include "Webserv.hpp"

/* Response es la clase que se encarga de gestionar las respuestas HTTP,
 las construye y prepara para ser usadas */

class Response
{
	private:
		const Server	*_server; // Servidor asociado
        std::string	_req_path;	// path recibido de la request
		std::string	_req_method; // metodo recibido de la request
        std::string	_real_location; // Ubicación del archivo despues de revisar las locations
        std::string	_content; // Contenido de la respuesta (incluidos headers)
        size_t		_content_length; // Tamaño del contenido
		std::string	_content_type; // Tipo de contenido
        std::string	_body; // Cuerpo de la respuesta
		std::string	_status_message; // Mensaje de estado
        short		_status_code; // Código de estado
		bool		_auto_index;

		void	GetRealLocation();

	public:
		Response(const Request &req, const Server &server);
		Response(const Response &other);
		Response &operator=(const Response &other);
		~Response();

		void		GenerateResponse();
		std::string	GetResponse();
};


#endif