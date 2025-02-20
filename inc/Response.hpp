#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include "Webserv.hpp"

class Location;

/* Response es la clase que se encarga de gestionar las respuestas HTTP,
 las construye y prepara para ser usadas */

class Response
{
	private:
		static const std::map<std::string, std::string> _mime_types;
		const Server	*_server; // Servidor asociado
        std::string	_req_path;	// path recibido de la request
		std::string	_req_method; // metodo recibido de la request
        std::string	_real_location; // Ubicación del archivo despues de revisar las locations
        std::string	_content; // Contenido de la respuesta (incluidos headers)
        size_t		_content_length; // Tamaño del contenido
		std::string	_content_type; // Tipo de contenido
        std::string	_body; // Cuerpo de la respuesta
		std::string	_status_message; // Mensaje de estado
		std::string _index;
        short		_status_code; // Código de estado
		bool		_auto_index;
		bool		_is_dir;

		static const std::map<std::string, std::string>	&SetMIMETypes();
		void		CheckMatchingLocation();
		void		CheckMethod(const Location &location);
		void		ExhaustivePathCheck(const std::string &path);
		bool		HandleAutoIndex();
		void		GenerateAutoIndex(const std::string &path);
		void		GetContentType(const std::string &path);
		void		GetBody(std::string path);
		std::string	GetDate();

	public:
		Response(const Request &req, const Server &server);
		Response(const Response &other);
		Response &operator=(const Response &other);
		~Response();

		void		GenerateResponse();
		std::string	GetResponse();

		class ResponseErrorException : public std::exception
		{
			private:
			short	_code;

			public:
			ResponseErrorException(short code) throw() : _code(code) {}

			virtual ~ResponseErrorException() throw() {}

			short getCode() const throw()
			{
				return _code;
			}

			virtual const char* what() const throw()
			{
				return "Error generating HTTP Response";
			}
		};
};

#endif