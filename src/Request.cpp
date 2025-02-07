#include "../inc/Request.hpp"

Request::Request(void){
    _method = ""; // metodo http (GET, POST...)
    _path_req = ""; // ruta de recurso solicitado
	_host = ""; // dominio desde el que se solicita
    _user_agent = ""; // datos del cliente (mozilla ver...)
    _request_format = ""; // tipos de contenido que espera
	_language = ""; // idioma preferido del cliente
    _encoding = ""; // tipos de codificacion que maneja el cliente.
	_keep_alive = 1; // 1 mantener conexion viva 0 cerrarla
	_max_body_size = -1; // limite de conf.yaml de memoria
	_body_size = 0; // espacio que ocupa la solicitud
    _error_type = 404; // error de salida en caso de no estar bien la solicitud.
}

Request::Request(int max_body_size) { // usare esta con el dato del conf.yaml
    _method = "";
    _path_req = "";
	_host = "";
    _user_agent = "";
    _request_format = "";
	_language = "";
    _encoding = "";
	_keep_alive = -1;
	_max_body_size = max_body_size;
	_body_size = -1;
    _error_type = -1;
}

Request::~Request(void){
    return ;
}

void    Request::parseSetup(std::string _request) {
    std::istringstream  stream(_request);
    std::string         line;
    if (std::getline(stream, line)) {
    std::istringstream request_line(line);
    std::string method, path, version;
    this->setMethod(method);
    this->setPath(path);
    }
    (void)version;
    while (std::getline(stream, line) && !line.empty()) {
        std::istringstream header_stream(line);
        std::string header, value;
        std::getline(header_stream, header, ':');
        std::getline(header_stream, value);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        if (header == "Host") {
            this->setHost(value);
        } else if (header == "User-Agent") {
            this->setUserAgent(value);
        } else if (header == "Accept") {
            this->setReqFormat(value);
        } else if (header == "Accept-Language") {
            this->setLanguage(value);
        } else if (header == "Accept-Encoding") {
            this->setEncoding(value);
        } else if (header == "Connection") {
            if (value == "keep-alive") {
                this->setKeepAlive(1);
            } else {
                this->setKeepAlive(0);
            }
        } else if (header == "Content-Length") {
            _body_size = std::stoi(value);
        }
    }
}

// necesita la request en un buffer, maxbodysize y el host.
// mbs = max body size :D
// devuelve 1 si la solicitud es buena, _error_type(404, 405...) si esta mal.
int Request::parseRequest(std::string _request, int mbs, std::string host){
    this->parseSetup(_request);
    // printRequestClass(const &data);
    if (this->getBodySize() > this->getMaxBodySize()){ // check de maxbodysize.
        this->setErrorType(405); // error de bodysize
        this->clean();
        return (this->getErrorType());
    }
    if (this->verifyMethodHost(host) == 0){ // check de metodo y host.
        this->setErrorType(500); // error de host/metodo.
        this->clean();
        return (this->getErrorType());
    }
    if (access(this->getPath(), R_OK) == -1){ // check de ruta
        this->setErrorType(404); // error de ruta
        this->clean();
        return (this->getErrorType());
    }
    if (this->getMethod() == "POST" && this->getRequestFormat().empty()){ // si el metodo es post tiene que tener formato
        this->setErrorType(406); // error de formato de peticion.
        this->clean();
        return (this->getErrorType());
    }
    if (this->getUserAgent().empty()){ // userAgent vacio
        this->setErrorType(406); // error de formato de peticion.
        this->clean();
        return (this->getErrorType());
    }
    return (1);
}

void    Request::clean(void){
    this->_method = "";
    this->_path_req = "";
	this->_host = "";
    this->_user_agent = "";
    this->_request_format = "";
	this->_language = "";
    this->_encoding = "";
	this->_keep_alive = 1;
	this->_max_body_size = 0;
	this->_body_size = 0;
}

int Request::verifyMethodHost(std::string host){
    if (host != getHost())
        return (0);
    if (getMethod() == "GET")
        return (1);
    if (getMethod() == "POST")
        return (1);
    return (0);
}

std::string  &Request::getMethod(void){
    return (this->_method);
}

std::string &Request::getPath(void){
    return (this->_path_req);
}

std::string &Request::getHost(void){
    return (this->_host);
}

std::string &Request::getUserAgent(void){
    return (this->_user_agent);
}

std::string &Request::getRequestFormat(void){
    return (this->_request_format);
}

std::string &Request::getLanguage(void){
    return (this->_language);
}

std::string &Request::getEncoding(void){
    return (this->_encoding);
}

bool    Request::getKeepAliveState(void){
    return (this->_keep_alive);
}

size_t  Request::getMaxBodySize(void){
    return (this->_max_body_size);
}

size_t  Request::getBodySize(void){
    return (this->_body_size);
}

int Request::getErrorType(void){
    return (this->_error_type);
}
        
void    Request::setMethod(std::string &method){
    this->_method = method;
}

void    Request::setHost(std::string &host){
    this->_host = host;
}

void    Request::setUserAgent(std::string &useragent){
    this->_user_agent = useragent;
}

void    Request::setPath(std::string &path){
    this->_path = path;
}

void    Request::setBodySize(size_t size){
    this->_body_size = size;
}

void    Request::setErrorType(int error_type){
    this->_error_type = error_type;
}

void    Request::setReqFormat(std::string &reqformat){
    this->_request_format = reqformat;
}

void    Request::setEncoding(std::string &encoding){
    this->_encoding = encoding;
}

void    Request::setLanguage(std::string &language){
    this->_language = language
}

bool    Request::setKeepAlive(int to){
    this->_keep_alive = to;
}

void    printRequestClass(const Request &req){
    std::cout << "Method " << req.getMethod() << std::endl;
    std::cout << "Path " << req.getPath() << std::endl;
    std::cout << "Host " << req.getHost() << std::endl;
    std::cout << "User Agent " << req.getUserAgent() << std::endl;
    std::cout << "Request format " << req.getRequestFormat() << std::endl;
    std::cout << "Language " << req.getLanguage() << std::endl;
    std::cout << "Encoding " << req.getEncoding() << std::endl;
    std::cout << "Keep Alive val. " << req.getKeepAliveState() << std::endl;
    std::cout << "Max body size " << req.getMaxBodySize() << std::endl;
    std::cout << "Body size " << req.getBodySize() << std::endl;
    std::cout << "Error type " << req.getErrorType() << std::endl;
}