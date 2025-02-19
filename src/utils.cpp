#include "../inc/ServerMaganer.hpp"
#include <cstdlib>

const std::string notFoundHtml = R"(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>404 Not Found</title>
        <style>
            body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
            h1 { color: #d9534f; }
            p { color: #5bc0de; }
        </style>
    </head>
    <body>
        <h1>404 Not Found</h1>
        <p>The requested URL was not found on this server.</p>
        <hr>
        <p><a href="/">Go back to the homepage</a></p>
    </body>
    </html>
    )";    

std::string	errorResponse(int val){
	char num[42];
    itoa(val, num, 10);
	std::string response = "HTTP/1.1" << num << "Not Found\r\n"
                           "Date: Tue, 08 Feb 2025 14:00:00 GMT\r\n"
                           "Server: MySimpleServer/1.0\r\n"
                           "Content-Type: text/html; charset=UTF-8\r\n"
                           "Content-Length: " + std::to_string(notFoundHtml.size()) + "\r\n"
                           "Connection: close\r\n"
                           "\r\n" + notFoundHtml;
	return (response);
}

