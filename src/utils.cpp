#include "../inc/Webserv.hpp"

const std::string errorHtml01 = 
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "    <title>error: ";

const std::string errorHtml02 = 
    "</title>\n"
    "<style>\n"
    "    body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }\n"
    "    h1 { color: #d9534f; }\n"
    "    p { color: #5bc0de; }\n"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "    <h1>";

const std::string errorHtml03 = 
    "</h1>\n"
    "    <p>";

const std::string errorHtml04 = 
    "</p>\n"
    "    <hr>\n"
    "    <p><a href=\"/\">Go back to the homepage</a></p>\n"
    "</body>\n"
    "</html>\n";

std::string errorResponse(short val, std::string error_page)
{
    char buffer[50];
    sprintf(buffer, "%d", val);
    std::string num(buffer);

    std::string message;
    if (val == 413)
        message = "Request Entity Too Large.";
    else if (val == 400)
        message = "Bad Request";
    else if (val == 405)
        message = "Method Not Allowed";
    else if (val == 404)
        message = "Not Found";
    else if (val == 403)
        message = "Forbidden";
    else if (val == 500)
        message = "Internal Server Error";
    else if (val == 415)
        message = "Unsupported Media Type";

    std::string body;
    if (error_page != "")
        body = error_page;
    else
        body = errorHtml01 + num + errorHtml02 + "ERROR " + num + errorHtml03 + message + errorHtml04;
    std::size_t contentLength = body.size();

    std::ostringstream  response;
    response << "HTTP/1.1 " << val << " " << message << "\r\n";
    response << "Server: webserv/1.0\r\n";
    if (val == 415)
        response << "Accept: application/octet-stream, text/plain, multipart/form-data\r\n";
    response << GetDate() << "\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << contentLength << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << body;

    if (DEBUG_MODE)
        std::cout << GREEN << "Error response:\n" << response.str() << RESET <<std::endl;
    return response.str();
}


/*
    Obtiene la fecha actual en formato GMT.
*/
std::string GetDate()
{
    time_t      _time;
    struct tm   *timeinfo;
    char        buffer[100];

    time(&_time);
    timeinfo = localtime(&_time);

    strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    return ("Date: " + std::string(buffer));
}