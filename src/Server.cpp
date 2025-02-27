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
    //loc1.redirection = "/html/redir/hola.html";

    Location loc2;
    loc2.path = "/location2";
    loc2.methods.push_back("GET");
    loc2.methods.push_back("POST");
    loc2.root = "";
    loc2.index = "index.html";
    loc2.autoindex = true;
    //loc2.redirection = "https://github.com/";

    Location loc3;
    loc3.path = "/location3";
    loc3.methods.push_back("GET");
    loc3.methods.push_back("POST");
    loc3.root = "";
    loc3.index = "";
    loc3.autoindex = false;

    Location loc4;
    loc4.path = "/cgi-bin";
    loc4.methods.push_back("GET");
    loc4.methods.push_back("POST");
    loc4.root = "";
    loc4.index = "";
    loc4.autoindex = true;

    Location loc5;
    loc5.path = "/uploads";
    loc5.methods.push_back("GET");
    loc5.methods.push_back("POST");
    loc5.root = "";
    loc5.index = "";
    loc5.autoindex = true;

    this->locations.push_back(loc1);
    this->locations.push_back(loc2);
    this->locations.push_back(loc3);
    this->locations.push_back(loc4);

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