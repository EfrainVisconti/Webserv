#include "../inc/Request.hpp"

Request::Request(void){
    _method = ""; // metodo http (GET, POST...)
    _path_req = ""; // ruta de recurso solicitado
	_host = ""; // dominio desde el que se solicita
    _user_agent = ""; // datos del cliente (mozilla ver...)
    _request_format = ""; // tipos de contenido que espera
	_language = ""; // idioma preferido del cliente
    _encoding = ""; // tipos de codificacion que maneja el cliente.
	_keep_alive = 0; // 1 mantener conexion viva 0 cerrarla
	_max_body_size = 0; // limite de conf.yaml de memoria
	_body_size = 0; // espacio que ocupa la solicitud
    _error_type = -1; // error de salida en caso de no estar bien la solicitud.
    _content_type = ""; // tipo de contenido
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
    _content_type = "";
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
    //printRequestClass();

    if (this->getPath().empty() || this->getHost().empty()) {
        this->setErrorType(400);
        return (400);
    }
    if (this->verifyMethod() == 0) {
        this->setErrorType(405);
        return (405);
    }
    if (this->getBodySize() > 0 && this->getBody().empty()) {
        this->setErrorType(500);
        return (500);
    }
    if (this->getMethod() == "POST" && this->getBodySize() > this->getMaxBodySize()) {
        this->setErrorType(413);
        return (413);
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

    int content_length = 0;
    bool has_body = false;

    while (std::getline(stream, line) && line != "\r") {
        std::istringstream header_stream(line);
        std::string header, value;

        std::getline(header_stream, header, ':');
        std::getline(header_stream, value);

        size_t first_non_space = value.find_first_not_of(" \t\r");
        size_t last_non_space = value.find_last_not_of(" \t\r");
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
            this->setKeepAlive(value == "keep-alive");
        } else if (header == "Content-Length") {
            content_length = atoi(value.c_str());
            this->setBodySize(content_length);
            has_body = (content_length > 0);
        } else if (header == "Content-Type") {
            this->setContentType(value);
        }
    }

    if (has_body) {
        std::vector<char>   body(content_length);
        stream.read(body.data(), content_length);
        this->setBody(body);
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

short Request::getErrorType(void) const{
    return (this->_error_type);
}

const std::string&  Request::getContentType(void) const{
    return (this->_content_type);
}

const std::vector<char>&  Request::getBody(void) const{
    return (this->_body);
}

void    Request::setContentType(std::string &content_type){
    this->_content_type = content_type;
}

void    Request::setBody(std::vector<char> &body){
    this->_body = body;
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

void    Request::printRequestClass(){
    std::cout << "Method: " << this->getMethod() << std::endl;
    std::cout << "Path: " << this->getPath() << std::endl;
    std::cout << "Host: " << this->getHost() << std::endl;
    std::cout << "User Agent: " << this->getUserAgent() << std::endl;
    std::cout << "Request format: " << this->getRequestFormat() << std::endl;
    std::cout << "Language: " << this->getLanguage() << std::endl;
    std::cout << "Encoding: " << this->getEncoding() << std::endl;
    std::cout << "Keep Alive: " << this->getKeepAliveState() << std::endl;
    std::cout << "Max body size: " << this->getMaxBodySize() << std::endl;
    std::cout << "Body size: " << this->getBodySize() << std::endl;
    std::cout << "Error type: " << this->getErrorType() << std::endl;
    std::cout << "Content type: " << this->getContentType() << std::endl;
    std::cout << "Body size readed: " << this->getBody().size() << std::endl;
}