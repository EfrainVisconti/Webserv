
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
	_max_body_size = 0; // limite de conf.yaml de memoria
	_body_size = 0; // espacio que ocupa la solicitud
    _error_type = 400; // error de salida en caso de no estar bien la solicitud.
}

Request::Request(unsigned long max_body_size) {
    _method = "";
    _path_req = "";
    _host = "";
    _user_agent = "";
    _request_format = "";
    _language = "";
    _encoding = "";
    _keep_alive = 0;
    _max_body_size = max_body_size;
    _body_size = 0;
    _error_type = -1;
}

Request::~Request(void) {
    return;
}

short Request::parseRequest(std::string _request) {
    if (DEBUG_MODE)
	{
		std::cout << GREEN << "Request received:\n"
				  << _request << RESET << std::endl;
	}

    this->parseSetup(_request);

    if (this->getMethod() == "POST" && this->getBodySize() > this->getMaxBodySize()) { // check de maxbodysize.
        this->setErrorType(413); // error de bodysize
        return (413);
    }
    if (this->verifyMethod() == 0) { // check de metodo
        this->setErrorType(405); // error de metodo
        return (405);
    }
    if (this->getMethod() == "POST" && this->getRequestFormat().empty()) { // si el metodo es post tiene que tener formato
        this->setErrorType(400); // error de formato de peticion.
        return (400);
    }
    if (this->getUserAgent().empty()) { // userAgent vacio
        this->setErrorType(400); // error de formato de peticion.
        return (400);
    }
    return (200);
}

void Request::parseSetup(std::string _request) {
    std::istringstream stream(_request);
    std::string line;
    if (std::getline(stream, line)) {
        std::istringstream request_line(line);
        std::string method, path, version;
        request_line >> method >> path;
        this->setMethod(method);
        this->setPath(path);
    }
    while (std::getline(stream, line) && !line.empty()) {
        std::istringstream header_stream(line);
        std::string header, value;
        std::getline(header_stream, header, ':');
        std::getline(header_stream, value);
        size_t first_non_space = value.find_first_not_of(" \t");
        size_t last_non_space = value.find_last_not_of(" \t");
        if (first_non_space != std::string::npos)
            value = value.substr(first_non_space, last_non_space - first_non_space + 1);
        else
            value = "";
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
            if (value == "keep-alive\r") { // Añadí \r para que funcione
                this->setKeepAlive(1);
            } else {
                this->setKeepAlive(0);
            }
        } else if (header == "Content-Length") {
            this->setBodySize(atoi(value.c_str()));
        }
    }
}

int Request::verifyMethod(){
    if (getMethod() == "GET")
        return (1);
    if (getMethod() == "POST")
        return (1);
    if (getMethod() == "DELETE")
        return (1);
    return (0);
}

const std::string&    Request::getMethod(void) const{
    return (this->_method);
}

const std::string&    Request::getPath(void) const{
    return (this->_path_req);
}

const std::string&    Request::getHost(void) const{
    return (this->_host);
}

const std::string&    Request::getUserAgent(void) const{
    return (this->_user_agent);
}

const std::string&    Request::getRequestFormat(void) const{
    return (this->_request_format);
}

const std::string&    Request::getLanguage(void) const{
    return (this->_language);
}

const std::string&    Request::getEncoding(void) const{
    return (this->_encoding);
}

bool    Request::getKeepAliveState(void) const{
    return (this->_keep_alive);
}

size_t  Request::getMaxBodySize(void) const{
    return (this->_max_body_size);
}

size_t  Request::getBodySize(void) const{
    return (this->_body_size);
}

int Request::getErrorType(void) const{
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
    this->_path_req = path;
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
    this->_language = language;
}

void    Request::setKeepAlive(bool to){
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