#include "../inc/Webserv.hpp"

// Las directivas dentro de un bloque location sobrescriben las del bloque 
// server si aplican a la misma solicitud.

//Todos los root se asumen NO terminan en / (excepto root "/" propiamente)

    //class Location
		// std::string			path; DONE
		// std::string			root; DONE
		// bool				autoindex; DONE
		// std::string			index;
		// std::vector<std::string>	methods; DONE
		// std::string			redirection;

Response::Response(const Request &req, const Server &server) : _server(&server)
{
    _req_path = req.getPath(); // path recibido de la request
    _req_method = req.getMethod(); // metodo recibido de la request
    _real_location = ""; // Ubicación del archivo despues de revisar las locations
    _content = ""; // Contenido de la respuesta (incluidos headers)
    _content_length = 0; // Tamaño del contenido
    _content_type = ""; // Tipo de contenido
    _body = ""; // Cuerpo de la respuesta
    _status_message = ""; // Mensaje de estado
    _status_code = 200; // Codigo de estado
    _auto_index = false;
}

Response::~Response()
{

}

Response::Response(const Response &other) : _server(other._server)
{
    *this = other;
}

Response &Response::operator=(const Response &other)
{
    if (this != &other)
    {
        _server = other._server;
        _req_path = other._req_path;
        _req_method = other._req_method;
        _real_location = other._real_location;
        _content = other._content;
        _content_length = other._content_length;
        _content_type = other._content_type;
        _body = other._body;
        _status_message = other._status_message;
        _status_code = other._status_code;
        _auto_index = other._auto_index;
    }
    return *this;
}

//405 Method Not Allowed (HTTP 405)
short   Response::CheckMethod(const Location &location)
{
    for (short i = 0; i < location.methods.size(); i++)
    {
        if (_req_method == location.methods[i])
            return (200);
    }
    return (405);
}


void    Response::GetRealLocation()
{
    std::vector<Location>::const_iterator it = _server->locations.begin();
    for (; it != _server->locations.end(); ++it)
    {
        if (_req_path.find(it->path) == 0 && 
        (_req_path.length() == it->path.length() ||
        _req_path[it->path.length()] == '/'))
        {
            _auto_index = it->autoindex;
            _status_code = CheckMethod(*it);
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
    _real_location = _server->root + _req_path;
}


void    Response::GenerateResponse()
{
    GetRealLocation();
}

std::string Response::GetResponse()
{
    return _real_location;
}