#include "../inc/Webserv.hpp"
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

Request::Request(int max_body_size) {
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

Request::~Request(void) {
    return;
}

void Request::parseSetup(std::string _request, Request& req) { // Cambié const Request& a Request&
    std::istringstream stream(_request);
    std::string line;
    if (std::getline(stream, line)) {
        std::istringstream request_line(line);
        std::string method, path, version;
        request_line >> method >> path;
        req.setMethod(method);
        req.setPath(path);
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
            req.setHost(value);
        } else if (header == "User-Agent") {
            req.setUserAgent(value);
        } else if (header == "Accept") {
            req.setReqFormat(value);
        } else if (header == "Accept-Language") {
            req.setLanguage(value);
        } else if (header == "Accept-Encoding") {
            req.setEncoding(value);
        } else if (header == "Connection") {
            if (value == "keep-alive") {
                req.setKeepAlive(1);
            } else {
                req.setKeepAlive(0);
            }
        } else if (header == "Content-Length") {
            req.setBodySize(atoi(value.c_str()));
        }
    }
}

void Request::clean(void) {
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
    //std::cout << "method " << getMethod() << "\n";
    //std::cout << "host " << getHost() << "\n";
    //std::cout << "host " << host << "\n";
    //if (host != getHost())
    //    return (0);
    (void)host;
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

bool    Request::setKeepAlive(int to){
    this->_keep_alive = to;
    return true;
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