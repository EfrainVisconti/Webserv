#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include "Webserv.hpp"

/* Response es la clase que se encarga de gestionar las respuestas HTTP,
 las construye y prepara para ser usadas */

class Response
{
	private:

	public:
		Response();
		~Response();
};

std::string defaultResponse(void);

#endif