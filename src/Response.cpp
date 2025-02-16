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

Response::~Response()
{

}

void    Response::GenerateResponse()
{
    _body = "Hello World!";
}

std::string Response::GetResponse()
{
    std::string response = "HTTP/1.1 200 OK\r\n"
                           "Date: Tue, 08 Feb 2025 14:00:00 GMT\r\n"
                           "Server: webserv/1.0\r\n"
                           "Content-Type: text/html; charset=UTF-8\r\n"
                           "Content-Length: 12\r\n"
                           "Connection: close\r\n"
                           "\r\n" + _body;
    return (response);
}