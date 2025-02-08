#include "../inc/Webserv.hpp"

Server::Server()
{
	this->_port = 0;
	this->_host = 0;
	this->_server_name = "";
	this->_root = "";
	this->_client_max_body_size = 50000;
	this->_index = "";
	this->_listen_socket = 0;
}

Server::~Server() {}

Server::Server(const Server &other) {
    *this = other;
}

Server &Server::operator=(const Server &other) {
    if (this != &other) {
        this->_host = other._host;
        this->_port = other._port;
        this->_server_name = other._server_name;
        this->_root = other._root;
        this->_client_max_body_size = other._client_max_body_size;
        this->_index = other._index;
        this->_error_pages = other._error_pages;
        this->_server_address = other._server_address;
        this->_listen_socket = other._listen_socket;
    }
    return *this;
}
