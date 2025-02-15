#include "../inc/Webserv.hpp"
# include <fstream>

std::string defaultResponse(void) {
    std::ifstream file("docs/index.html");
    if (!file.is_open()) {
        return "Error al cargar la página.";
    }
    std::string response, line;
    while (std::getline(file, line)) {
        response += line + "\n";
    }
    int contentsize = response.size();
    std::stringstream ss;
    ss << contentsize;
    std::string contentsize_str = ss.str();
    std::string baseresponse = "HTTP/1.1 200 OK\r\n"
                               "Date: Tue, 08 Feb 2025 14:00:00 GMT\r\n"
                               "Server: WebServ/1.0\r\n"
                               "Content-Type: text/html; charset=UTF-8\r\n"
                               "Content-Length: " + contentsize_str + "\r\n"
                               "Connection: close\r\n\r\n"
                               + response;
    file.close();
    return baseresponse;
}
