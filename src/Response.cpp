#include "../inc/Webserv.hpp"

// Las directivas dentro de un bloque location sobrescriben las del bloque 
// server si aplican a la misma solicitud.

//Todos los root se asumen NO terminan en / (excepto root "/" propiamente)

    //class Location
		// std::string			path;
		// std::string			root;
		// bool				autoindex;
		// std::string			index;
		// std::vector<short>	methods;
		// std::string			redirection;

Response::Response(const Request &req, const Server &server) : _server(server)
{
    _req_path = req.getPath(); // path recibido de la request
    _req_method = req.getMethod(); // metodo recibido de la request
    _real_location = ""; // Ubicación del archivo despues de revisar las locations
    _content = ""; // Contenido de la respuesta (incluidos headers)
    _content_length = 0; // Tamaño del contenido
    _content_type = ""; // Tipo de contenido
    _body = ""; // Cuerpo de la respuesta
    _status_message = ""; // Mensaje de estado
    _status_code = 0; // Codigo de estado
    _auto_index = false;
}


Response::~Response()
{

}


void    Response::GetRealLocation()
{
    std::vector<Location>::const_iterator it = _server.locations.begin();
    for (; it != _server.locations.end(); ++it)
    {
        std::cout << it->root << std::endl;
        std::cout << _req_path << std::endl;
        std::cout << _real_location << std::endl;
        if (_req_path.find(it->path) == 0 && 
        (_req_path.length() == it->path.length() ||
        _req_path[it->path.length()] == '/'))
        {
            std::cout << it->root << std::endl;
            std::cout << _req_path << std::endl;
            std::cout << _real_location << std::endl;
            _auto_index = it->autoindex;
            if (it->root == "/")
            {
                _real_location = _req_path;
                return ;
            }
            if (it->root == "")
                break ;
            _real_location = it->root + _req_path.substr(it->path.length());
            return ;
        }
    }
    std::cout << _req_path << std::endl;
    _real_location = _server.root + _req_path;
}


void    Response::GenerateResponse()
{
    GetRealLocation();
}

std::string Response::GetResponse()
{
    return _real_location;
}