#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include "Request.hpp"
# include "Webserv.hpp"

class Location;
class Request;
class Server;

/* Response es la clase que se encarga de gestionar las respuestas HTTP,
 las construye y prepara para ser usadas */

class Response
{
	private:
		static const std::map<std::string, std::string> _mime_types;
		static short	post_files;

		const Request	*_request; // Petición asociada
		const Server	*_server; // Servidor asociado
        std::string	_req_path;	// path recibido de la request
		std::string	_req_method; // metodo recibido de la request
        std::string	_real_location; // Ubicación del archivo despues de revisar las locations
        std::string	_response; // Contenido de la respuesta (incluidos headers)
        size_t		_content_length; // Tamaño del contenido
		std::string	_content_type; // Tipo de contenido
        std::string	_body; // Cuerpo de la respuesta
		std::string	_status_message; // Mensaje de estado
		std::string _index;
        short		_status_code; // Código de estado
		bool		_auto_index;
		bool		_is_dir;
		bool		_is_loc;

		Response();
		static const std::map<std::string, std::string>	&SetMIMETypes();
		void		InitialStatusCodeCheck();
		void		CheckMatchingLocation();
		void		CheckMethod(const Location &location);
		void		ExhaustivePathCheck(const std::string &path);
		bool		HandleAutoIndex();
		void		GenerateAutoIndex(const std::string &path);
		void		GetContentType(const std::string &path);
		std::string	GetBody(std::string path);
		void		SetResponse(bool status);
		bool		HTTPRedirectionCase();
		void		HandleDelete(const std::string &path);
		void		HandlePost(const std::string &content_type);
		std::string	RemoveBoundary(const std::string &body, const std::string &boundary);
		std::vector<char>	RemoveBoundary(const std::vector<char> &body, const std::string &boundary);
		std::string	GetFilename(const std::vector<char> &body);
		void		HandleCgi();
		std::string	getQueryString();
		int			parseCgi();
		int			notHtml();
		static void	timeout_handler(int sig);
		template <typename T>
		void	CreateFile(const T &body, const std::string &boundary, const std::string &filename);

		public:
		Response(const Request &req, const Server &server, short status);
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