#include "../inc/Webserv.hpp"
#include <cstdlib>

const std::string errorHtml01 = R"(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>error: )";

const std::string errorHtml02 = R"(</title>
        <style>
            body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
            h1 { color: #d9534f; }
            p { color: #5bc0de; }
        </style>
    </head>
    <body>
        <h1>)";

const std::string errorHtml03 = R"(
        "</h1>
        <p>)";

const std::string errorHtml04 = R"(</p>
        <hr>
        <p><a href="/">Go back to the homepage</a></p>
    </body>
    </html>
)";

std::string	errorResponse(int val){
    char buffer[50];
    std::sprintf(buffer, "%d", val);
	std::string num(buffer);
    if (num == 404){
        std::string message = "Unable to find HTTP Request path.";
    }
    if (num == 405){
        std::string message = "Request body size exceed 'config' max body size.";
    }
    if (num == 406){
        std::string message = "POST format error.";
    }
    if (num == 500){
        std::string message = "Method/Host error.";
    }
	std::string response = "HTTP/1.1 " + num + " Not Found\r\n"
                           "Date: Tue, 08 Feb 2025 14:00:00 GMT\r\n"
                           "Server: MySimpleServer/1.0\r\n"
                           "Content-Type: text/html; charset=UTF-8\r\n"
                           "Content-Length: " + std::to_string(errorHtml01.size() + errorHtml02.size()) + "\r\n"
                           "Connection: close\r\n"
                           "\r\n" + errorHtml01 + num + errorHtml02 + "ERROR " + num + message;
	return (response);
}
