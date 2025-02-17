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
    
    Location location1;
    location1.path = "/location1";
    location1.methods.push_back("GET");
    location1.methods.push_back("POST");
    location1.root = "/";
    location1.index = "index_location1.html";
    location1.autoindex = false;

    Location location2;
    location2.path = "/location2";
    location2.methods.push_back("GET");
    location2.methods.push_back("POST");
    //location2.root = "/location2";
    location2.index = "index_location2.html";
    location2.autoindex = true;

    Location location3;
    location3.path = "/kapouet";
    location3.methods.push_back("GET");
    location3.methods.push_back("POST");
    location3.methods.push_back("DELETE");
    location3.root = "/tmp/www";
    location3.index = "index.html";
    location3.autoindex = false;

    this->locations.push_back(location1);
    this->locations.push_back(location2);
    this->locations.push_back(location3);

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