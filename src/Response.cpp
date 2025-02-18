#include "../inc/Webserv.hpp"

//301 Moved Permanently (HTTP 301)
//405 Method Not Allowed (HTTP 405)

// Las directivas dentro de un bloque location sobrescriben las del bloque 
// server si aplican a la misma solicitud.

//Todos los root se asumen NO terminan en / (excepto root "/" propiamente)

    //class Location
		// std::string			path; DONE
		// std::string			root; DONE
		// bool				autoindex; QUASI DONE
		// std::string			index; Se gestiona segun el metodo
		// std::vector<std::string>	methods; DONE
		// std::string			redirection; DONE

// HTTP/1.1 200 OK
// Server: nginx/1.21.3
// Date: Fri, 14 Feb 2025 12:00:00 GMT
// Content-Type: text/html; charset=UTF-8
// Content-Length: 1234
// Connection: keep-alive
// Last-Modified: Tue, 13 Feb 2025 10:00:00 GMT
// ETag: "5d8c72a5-4d2"
// Accept-Ranges: bytes

// <html>
//   <head><title>Welcome to NGINX</title></head>
//   <body><h1>Hello, world!</h1></body>
// </html>



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

/*
    Revisa si el método de la request esta permitido en la location.
*/
void    Response::CheckMethod(const Location &location)
{
    if (std::find(location.methods.begin(), location.methods.end(), _req_method) == location.methods.end())
        throw Response::ResponseErrorException(405);
}

/*
    Establece la ubicación real del recurso solicitado teniendo en cuenta las location en el
    archivo de configuración del server.

    Dada una location /location1 y una request /location1/file.html. Si en la location1:
    1. root / --> sirve /location1/file.html (empieza desde la raíz del sistema)
    2. root inexistente --> sirve /html/location1/file.html (empieza desde la raíz del 
    server. Ej: /html)
    3. root /html/pages --> sirve /html/pages/file.html (sin location1 en la ruta)

    Se llama a CheckMethod(), la cual lanza 405 si el método no esta permitido en la location.

    Si la location tiene una redirección, se establece el status code a 301 y se establece
    la nueva ubicación.
    Se establece el autoindex de la location (false por defecto).

*/
void    Response::CheckMatchingLocation()
{
    std::vector<Location>::const_iterator it = _server->locations.begin();
    for (; it != _server->locations.end(); ++it)
    {
        if (_req_path.find(it->path) == 0 && 
        (_req_path.length() == it->path.length() ||
        _req_path[it->path.length()] == '/'))
        {
            CheckMethod(*it);
            if (it->redirection != "")
            {
                _status_code = 301;
                _real_location = it->redirection;
                return ;
            }
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
    _real_location = _server->root + _req_path;
}


short   Response::CheckPath(const std::string &path)
{
    struct stat sb;

    if (stat(path.c_str(), &sb) != 0)
        return PATH_NOT_FOUND;
    if (S_ISREG(sb.st_mode))
        return PATH_IS_FILE;
    if (S_ISDIR(sb.st_mode))
        return PATH_IS_DIRECTORY;
    return PATH_NOT_FOUND;
}


void    Response::GenerateResponse()
{
    CheckMatchingLocation();

    short path_status = CheckPath(_real_location);
    if (path_status == PATH_NOT_FOUND)
        throw Response::ResponseErrorException(404);
    if (path_status == PATH_IS_DIRECTORY)

}


std::string Response::GetResponse()
{
    return _real_location;
}