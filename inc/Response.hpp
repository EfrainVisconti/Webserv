#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include "Webserv.hpp"

/* Response es la clase que se encarga de gestionar las respuestas HTTP,
 las construye y prepara para ser usadas */

class Response
{
	private:
		const Server	&_server; // Servidor asociado
        std::string	_req_path;	// path recibido de la request
		std::string	_req_method; // metodo recibido de la request
        std::string	_location; // Ubicación del archivo
        std::string	_content; // Contenido de la respuesta
        size_t		_content_length; // Tamaño del contenido
		std::string	_content_type; // Tipo de contenido
        std::string	_body; // Cuerpo de la respuesta
		std::string	_status_message; // Mensaje de estado
        short		_status_code; // Código de estado
		bool		_auto_index;

	public:
		Response(const Request &req, const Server &server);
		~Response();

		void		GenerateResponse();
		std::string	GetResponse();
};


#endif