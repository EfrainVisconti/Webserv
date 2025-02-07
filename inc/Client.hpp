#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Webserv.hpp"

/* Client almacena la informacion relacionada a un cliente, como su
 socket y el servidor al que esta conectado */

class Client
{
    public:
        Client();
        ~Client();

        Response	response;
        Request		request;
        Server		server;
        int			client_socket;
        struct sockaddr_in	client_address;

	private:

};


#endif