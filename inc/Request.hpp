#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Webserv.hpp"

/* Request es la clase que se encarga de gestionar las peticiones HTTP,
 parsea y almacena la información */

class Request
{
	private:
		std::string							_method;
		std::string							_path_req;
		std::string							_host;
		std::string							_user_agent;
		std::string							_request_format; // accept
		std::string							_language;
		std::string							_encoding;
		bool								_keep_alive;
		size_t								_max_body_size;
		size_t								_body_size;
		int									_error_type;
	public:
		Request();
		Request(int max_body_size);
		~Request();
		void					parseSetup(std::string _request);
		void					clean();
		int						verifyMethodHost(std::string host);
	// getters
		std::string				&getMethod();
		std::string				&getPath();
		std::string				&getHost();
		std::string				&getUserAgent();
		std::string				&getRequestFormat();
		std::string				&getLanguage();
		std::string				&getEncoding();
		bool					getKeepAliveState();
		size_t					getMaxBodySize();
		size_t					getBodySize();
		int						getErrorType();
	// setters
		void					setMethod(std::string &method);
		void					setBodySize(size_t size);
		void					setErrorType(int error_type);
		bool					setKeepAlive(int to);

	int		parseRequest(std::string request, int mbs, std::string host);
};

	void	printRequestClass(const Request &req);

#endif