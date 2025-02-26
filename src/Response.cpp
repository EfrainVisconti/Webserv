#include "../inc/Webserv.hpp"

    //class Location
		// std::string			path; DONE
		// std::string			root; DONE
		// bool				autoindex; DONE
		// std::string			index; DONE
		// std::vector<std::string>	methods; DONE
		// std::string			redirection; DONE

const std::map<std::string, std::string>    Response::_mime_types = Response::SetMIMETypes();

Response::Response(const Request &req, const Server &server, short status) : _server(&server), _status_code(status)
{
    _req_path = req.getPath(); // path recibido de la request
    _req_method = req.getMethod(); // metodo recibido de la request
    _real_location = ""; // Ubicación del archivo despues de revisar las locations
    _response = ""; // Contenido de la respuesta (incluidos headers)
    _content_length = 0; // Tamaño del contenido
    _content_type = "text/plain"; // Tipo de contenido
    _body = ""; // Cuerpo de la respuesta
    _status_message = "OK"; // Mensaje de estado
    _auto_index = false; // off por defecto
    _is_dir = false;
    _index = _server->index;
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
        _response = other._response;
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
    Las directivas dentro de un bloque location sobrescriben las del bloque server si aplican a
    la misma solicitud.
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
    Si la location tiene un index, se establece el index de la location para la response.
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
                _status_message = "Moved Permanently";
                _real_location = it->redirection;
                return ;
            }
            _auto_index = it->autoindex;
            _index = it->index;
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
    std::string path_aux = path.substr(1);
    DIR *dir = opendir(path_aux.c_str());
    if (dir == NULL)
        throw Response::ResponseErrorException(500);

    struct dirent *entry;
    std::vector<std::string> files;

    while ((entry = readdir(dir)) != NULL)
    {
        std::string name = entry->d_name;
        if (name == "." || name == "..")
            continue ;

        struct stat st;
        std::string full_path = path_aux + "/" + name;
        if (stat(full_path.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
            name += "/";

        files.push_back(name);
    }
    closedir(dir);

    std::ostringstream html;
    html << "<html><head><title>Index of " << path << "</title></head><body>";
    html << "<h1>Index of " << path << "</h1><hr><ul>";

    for (std::size_t i = 0; i < files.size(); i++)
        html << "<li><a href=\"" << files[i] << "\">" << files[i] << "</a></li>";

    html << "</ul><hr></body></html>";
    _body = html.str();
    _content_length = _body.length();
    _content_type = "text/html";
}


/*
    En el archivo de configuración se establece index para el server y para cada location. Si no se
    encuentra index en las configuraciones, se establece index inexistente ("").
    El index debe estar alojado en el directorio al que se esta intentando acceder.
    Para generar el autoindex debe estar activado el autoindex (true) y no tener index configu.
*/
bool    Response::HandleAutoIndex()
{
    if (_index == "")
    {
        if (_auto_index == true)
        {
            GenerateAutoIndex(_real_location);
            return true;
        }
        throw Response::ResponseErrorException(403);
    }
    _real_location = _real_location + _index;
    return false;
}


std::string Response::GetBody(std::string path)
{
    path = path.substr(1);
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file)
        throw Response::ResponseErrorException(404);

    std::ostringstream content;
    content << file.rdbuf();
    file.close();
    _body = content.str();
    _content_length = _body.length();
    return _body;
}


void    Response::GetContentType(const std::string &path)
{
    std::string extension = path.substr(path.find_last_of(".") + 1);
    std::map<std::string, std::string>::const_iterator it = _mime_types.find(extension);
    if (it != _mime_types.end())
        _content_type = it->second;
}


bool    Response::HTTPRedirectionCase()
{
    if (_req_method == "GET" && _status_code == 301
        && (_real_location.rfind("http://", 0) == 0 ||
        _real_location.rfind("https://", 0) == 0))
    {
        SetResponse(true);
        return true;
    }
    _status_code = 200; // Para redirecciones internas
    return false;
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
    std::string path_aux = path.substr(1);
    struct stat buffer;
    if (stat(path_aux.c_str(), &buffer) == -1)
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
            _real_location = path + "/";
        return ;
    }

    int open_return = open(path_aux.c_str(), O_RDONLY);
    if (open_return == -1)
    {
        if (errno == EACCES)
            throw Response::ResponseErrorException(403);
        throw Response::ResponseErrorException(500);
    }
    close(open_return);
}


void    Response::InitialStatusCodeCheck()
{
    if (_status_code == 400)
        throw Response::ResponseErrorException(400);
    if (_status_code == 405)
        throw Response::ResponseErrorException(405);
    if (_status_code == 413)
        throw Response::ResponseErrorException(413);
    if (_status_code == 500)
        throw Response::ResponseErrorException(500);
}


void	Response::HandleDelete(const std::string &path)
{
    std::string path_aux = path.substr(1);
    if (std::remove(path_aux.c_str()) == 0)
    {
        _status_code = 204;
        _status_message = "No Content";
        _content_type = "";
        _content_length = 0;
        _body = "";
    }
    else
        throw Response::ResponseErrorException(500);

}


void    Response::GenerateResponse()
{
    try
    {
        InitialStatusCodeCheck();
        if (_req_path.find("/cgi-bin") == 0 && parseCgi() == true)
		{
            HandleCgi();
            SetResponse(true);
            return ;
        }
        CheckMatchingLocation();
        if (HTTPRedirectionCase() == true)
        {
            SetResponse(true);
            return ;
        }
        ExhaustivePathCheck(_real_location);
		if (_req_method == "GET")
		{
            if (_is_dir == true)
            {
                if (HandleAutoIndex() == true)
                {
                    SetResponse(true);
                    return ;
                }
            }
            GetContentType(_real_location);
            GetBody(_real_location);
            SetResponse(true);
			return ;
		}
		if (_req_method == "DELETE")
		{
            if (_is_dir == true)
                throw Response::ResponseErrorException(403);
			HandleDelete(_real_location);
            SetResponse(true);
            return ;
		}

    }
	catch (const Response::ResponseErrorException &e)
	{
		_status_code = e.getCode();
        SetResponse(false);
	}
}


void    Response::SetResponse(bool status)
{
    std::cout << "Req path: " << _req_path << std::endl;
    std::cout << "Real location: " << _real_location << std::endl;
    if (status)
    {
        std::ostringstream  aux;
        aux << "HTTP/1.1 " << _status_code << " " << _status_message << "\r\n";
        aux << "Server: webserv/1.0\r\n";
        if (_status_code == 301)
            aux << "Location: " << _real_location << "\r\n";
        aux << ::GetDate() << "\r\n";
        aux << "Content-Type: " << _content_type << "\r\n";
        aux << "Content-Length: " << _content_length << "\r\n";
        aux << "Connection: close\r\n";
        aux << "\r\n";
        aux << _body;

        _response = aux.str();

        if (DEBUG_MODE)
            std::cout << GREEN << "Successful response:\n" << _response << RESET <<std::endl;
    }
    else
    {
        std::map<short, std::string>::const_iterator it = _server->error_pages.find(_status_code);
        if (it != _server->error_pages.end())
            _response = errorResponse(_status_code, GetBody(it->second));
        else
            _response = errorResponse(_status_code, "");
    }
}


std::string Response::GetResponse()
{
    return _response;
}


const std::map<std::string, std::string>  &Response::SetMIMETypes()
{
    static std::map<std::string, std::string>  aux;

    if (aux.empty())
    {
        aux["html"] = "text/html";
        aux["css"] = "text/css";
        aux["js"] = "application/javascript";
        aux["jpg"] = "image/jpeg";
        aux["jpeg"] = "image/jpeg";
        aux["png"] = "image/png";
        aux["gif"] = "image/gif";
        aux["mp4"] = "video/mp4";
        aux["mp3"] = "audio/mpeg";
        aux["txt"] = "text/plain";
        aux["json"] = "application/json";
        aux["pdf"] = "application/pdf";
    }
    return aux;
}