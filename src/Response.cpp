#include "../inc/Webserv.hpp"

//301 Moved Permanently (HTTP 301)
//405 Method Not Allowed (HTTP 405)
//404 Not Found (HTTP 404)
//403 Forbidden (HTTP 403)
//500 Internal Server Error (HTTP 500)

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
    _auto_index = false; // off por defecto
    _is_dir = false;

    if (server.index != "")
        _index = server.root + "/" + server.index;
    else
        _index = "";
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
        _is_dir = other._is_dir;
        _index = other._index;
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
    Si la location tiene un index, se establece el index de la location para la response,
    el cual debe estar alojado en la raíz de la location.
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
            if (it->index != "")
                _index = it->root + "/" + it->index;
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


void    Response::GenerateAutoIndex(const std::string &path)
{
    DIR *dir = opendir(path.c_str());
    if (dir == NULL)
        throw Response::ResponseErrorException(500);

    struct dirent *entry;
    std::vector<std::string> files;

    while ((entry = readdir(dir)) != NULL)
    {
        if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..")
            files.push_back(entry->d_name);
    }
    closedir(dir);

    std::sort(files.begin(), files.end());

    std::ostringstream html;
    html << "<html><head><title>Index of " << path << "</title></head><body>";
    html << "<h1>Index of " << path << "</h1><hr><ul>";

    for (std::size_t i = 0; i < files.size(); i++)
    {
        std::string filePath = path + "/" + files[i];
        struct stat buffer;
        stat(filePath.c_str(), &buffer);
        if (S_ISDIR(buffer.st_mode))
        {
            html << "<li><a href=\"" << files[i] << "/\">" << files[i] << "/</a></li>";
        }
        else
        {
            html << "<li><a href=\"" << files[i] << "\">" << files[i] << "</a></li>";
        }
    }

    html << "</ul><hr></body></html>";
    _body = html.str();
}


/*
    En el archivo de configuración se establece index para el server y para cada location. Si no se
    encuentra index en las configuraciones, se establece index inexistente ("").
    El index debe estar alojado en la raíz de la location o en la raiz del Server si no hay
    location para esa request.
*/
void    Response::HandleAutoIndex()
{
    if (_index == "")
    {
        if (_auto_index == true)
        {
            GenerateAutoIndex(_real_location);
            return ;
        }
        throw Response::ResponseErrorException(403);
    }
    _real_location = _index;
}


/*
    Revisa exhaustivamente si la ubicación del recurso solicitado existe, es un directorio
    o es un archivo. Se revisa la existencia de la ubicación con stat() y si no es un directorio,
    se intenta abrir el archivo con open().

    - Si la ubicación no existe, se establece el status code a 404.
    - Si la ubicación es un directorio y no termina en /, se establece el status code a 301 y se
    establece la nueva ubicación con / al final.
    - Si la ubicación es un archivo y no se puede abrir, se establece el status code a 403.
    - En cualquier otro caso de error, se establece el status code a 500.
*/
void    Response::ExhaustivePathCheck(const std::string &path)
{
    struct stat buffer;
    if (stat(path.c_str(), &buffer) == -1)
    {
        if (errno == ENOENT)
            throw Response::ResponseErrorException(404);
        if (errno == EACCES)
            throw Response::ResponseErrorException(403);
        throw Response::ResponseErrorException(500);
    }

    if (S_ISDIR(buffer.st_mode))
    {
        _is_dir = true;
        if (path[path.length() - 1] != '/')
        {
            _status_code = 301;
            _real_location = path + "/";
        }
        return;
    }

    int open_return = open(path.c_str(), O_RDONLY);
    if (open_return == -1)
    {
        if (errno == EACCES)
            throw Response::ResponseErrorException(403);
        throw Response::ResponseErrorException(500);
    }
    close(open_return);
}


void    Response::GenerateResponse()
{
    try
    {
        if (_req_method == "GET")
        {
            CheckMatchingLocation();
            ExhaustivePathCheck("/home/usuario/Documents/Webserv" + _real_location);
            if (_is_dir == true)
                HandleAutoIndex();
        }

    }
	catch (const Response::ResponseErrorException &e)
	{
		_status_code = e.getCode();
	}

}


std::string Response::GetResponse()
{
    std::ostringstream response;
    response << _req_path << "\r\n";
    response << _req_method << "\r\n";
    response << _real_location << "\r\n";
    response << _content << "\r\n";
    response << "HTTP/1.1 " << _status_code << " " << _status_message << "\r\n";
    response << "Server: webserv/1.0\r\n";
    response << "Date: " << "Fri, 14 Feb 2025 12:00:00 GMT" << "\r\n";
    response << "Content-Type: " << _content_type << "\r\n";
    response << "Content-Length: " << _content_length << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << _body;
    return response.str();
}
