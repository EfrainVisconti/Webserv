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
    else if (val == 414)
        message = "URI Too Long";

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
    {
        response << "Accept: multipart/form-data, text/html, text/css, application/javascript, ";
        response << "image/jpeg, image/png, image/gif, video/mp4, audio/mpeg, ";
        response << "text/plain, application/json, application/pdf, application/octet-stream\r\n";
    }
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

std::string statusCodeString(short statusCode)
{
    switch (statusCode)
    {
        case 100:
            return "Continue";
        case 101:
            return "Switching Protocol";
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 202:
            return "Accepted";
        case 203:
            return "Non-Authoritative Information";
        case 204:
            return "No Content";
        case 205:
            return "Reset Content";
        case 206:
            return "Partial Content";
        case 300:
            return "Multiple Choice";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Moved Temporarily";
        case 303:
            return "See Other";
        case 304:
            return "Not Modified";
        case 307:
            return "Temporary Redirect";
        case 308:
            return "Permanent Redirect";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 406:
            return "Not Acceptable";
        case 407:
            return "Proxy Authentication Required";
        case 408:
            return "Request Timeout";
        case 409:
            return "Conflict";
        case 410:
            return "Gone";
        case 411:
            return "Length Required";
        case 412:
            return "Precondition Failed";
        case 413:
            return "Payload Too Large";
        case 414:
            return "URI Too Long";
        case 415:
            return "Unsupported Media Type";
        case 416:
            return "Requested Range Not Satisfiable";
        case 417:
            return "Expectation Failed";
        case 418:
            return "I'm a teapot";
        case 421:
            return "Misdirected Request";
        case 425:
            return "Too Early";
        case 426:
            return "Upgrade Required";
        case 428:
            return "Precondition Required";
        case 429:
            return "Too Many Requests";
        case 431:
            return "Request Header Fields Too Large";
        case 451:
            return "Unavailable for Legal Reasons";
        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        case 504:
            return "Gateway Timeout";
        case 505:
            return "HTTP Version Not Supported";
        case 506:
            return "Variant Also Negotiates";
        case 507:
            return "Insufficient Storage";
        case 510:
            return "Not Extended";
        case 511:
            return "Network Authentication Required";
        default:
            return "Undefined";
        }
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

int ft_stoi(std::string str)
{
    std::stringstream ss(str);
    if (str.length() > 10)
        throw std::exception();
    for (size_t i = 0; i < str.length(); ++i)
    {
        if(!isdigit(str[i]))
            throw std::exception();
    }
    int res;
    ss >> res;
    return (res);
}

unsigned int fromHexToDec(const std::string& nb)
{
	unsigned int x;
	std::stringstream ss;
	ss << nb;
	ss >> std::hex >> x;
	return (x);
}

void printIPv4(uint32_t ip) 
{
    uint8_t byte1 = (ip >> 24) & 0xFF;
    uint8_t byte2 = (ip >> 16) & 0xFF;
    uint8_t byte3 = (ip >> 8) & 0xFF;
    uint8_t byte4 = ip & 0xFF;

    std::cout << static_cast<int>(byte4) << "."
              << static_cast<int>(byte3) << "."
              << static_cast<int>(byte2) << "."
              << static_cast<int>(byte1) << std::endl;
}
