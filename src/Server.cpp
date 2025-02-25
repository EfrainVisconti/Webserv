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
    
    Location loc1;
    loc1.path = "/location1";
    loc1.methods.push_back("GET");
    loc1.methods.push_back("POST");
    loc1.methods.push_back("DELETE");
    loc1.root = "";
    loc1.index = "index.html";
    loc1.autoindex = false;

    Location loc2;
    loc2.path = "/location2";
    loc2.methods.push_back("GET");
    loc2.methods.push_back("POST");
    loc2.root = "";
    loc2.index = "";
    loc2.autoindex = true;

    Location loc3;
    loc3.path = "/cgi-bin";
    loc3.methods.push_back("GET");
    loc3.methods.push_back("POST");
    loc3.root = "/cgi-bin";
    loc3.index = "";
    loc3.autoindex = false;

    this->locations.push_back(loc1);
    this->locations.push_back(loc2);
    this->locations.push_back(loc3);

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
        this->locations = other.locations;
    }
    return *this;
}