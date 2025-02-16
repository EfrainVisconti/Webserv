#include "../inc/Webserv.hpp"

Response::Response(const Request &req, const Server &server) : _server(server)
{
    _req_path = req.getPath();
    _req_method = req.getMethod();
    _location = "";
    _content = "";
    _content_length = 0;
    _content_type = "";
    _body = "";
    _location = "";
    _status_message = "";
    _status_code = 0;
    _auto_index = false;
}

Response::~Response(){
}

void    Response::GenerateResponse()
{
    std::string filename = "./html/index.html";
    std::ifstream file (filename.c_str());
    std::stringstream buff;
    buff << file.rdbuf();
    std::string htmlCont = buff.str();
    file.close();
    _body = htmlCont;
}

std::string Response::GetResponse()
{
    std::ostringstream oss;
    oss << _body.size();
    std::string num = oss.str();
    std::string response = "HTTP/1.1 200 OK\r\n"
                           "Date: Tue, 08 Feb 2025 14:00:00 GMT\r\n"
                           "Server: webserv/1.0\r\n"
                           "Content-Type: text/html; charset=UTF-8\r\n"
                           "Content-Length: " + num + "\r\n"
                           "Connection: close\r\n"
                           "\r\n" + _body;
    return (response);
}