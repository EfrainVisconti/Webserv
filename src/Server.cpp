#include "../inc/Webserv.hpp"

Server::Server()
{
	this->port = 0;
	this->host = 0;
	this->server_name = "";
	this->root = "";
	this->client_max_body_size = 50000;
	this->index = "";
	this->listen_socket = 0;
}

Server::~Server() {}

Server::Server(const Server &other)
{
    *this = other;
}

Server &Server::operator=(const Server &other)
{
    if (this != &other)
    {
        this->host = other.host;
        this->port = other.port;
        this->server_name = other.server_name;
        this->root = other.root;
        this->client_max_body_size = other.client_max_body_size;
        this->index = other.index;
        this->error_pages = other.error_pages;
        this->server_address = other.server_address;
        this->listen_socket = other.listen_socket;
    }
    return *this;
}
