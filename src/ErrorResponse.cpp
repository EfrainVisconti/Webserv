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

std::string errorResponse(int val) {
    char buffer[50];
    sprintf(buffer, "%d", val);
    std::string num(buffer);

    std::string message = "";
    if (val == 413) {
        message = "Request Entity Too Large.";
    }
    if (val == 406) {
        message = "POST format error.";
    }
    if (val == 500) {
        message = "Method/Host error.";
    }

    // Calcular Content-Length (reemplazo de std::to_string)
    std::ostringstream contentLengthStream;
    contentLengthStream << (errorHtml01.size() + errorHtml02.size() + errorHtml03.size() + errorHtml04.size() + 100);
    std::string contentLength = contentLengthStream.str();

    // Construcción de la respuesta HTTP (sin std::to_string)
    std::string response = "HTTP/1.1 " + num + " Not Found\r\n"
                           "Date: Tue, 08 Feb 2025 14:00:00 GMT\r\n"
                           "Server: webserv/1.0\r\n"
                           "Content-Type: text/html; charset=UTF-8\r\n"
                           "Content-Length: " + contentLength + "\r\n"
                           "Connection: close\r\n"
                           "\r\n" + errorHtml01 + num + errorHtml02 + "ERROR " + num + errorHtml03 + message + errorHtml04;

    std::cout << response << std::endl;
    return response;
}
