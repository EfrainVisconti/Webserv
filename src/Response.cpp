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
        <h1>404 Not Found</h1>
        <p>The requested URL was not found on this server.</p>
        <hr>
        <p><a href="/">Go back to the homepage</a></p>
    </body>
    </html>
    )
)";

std::string defaultResponse(void){
    std::string defaultR = R"(HTTP/1.1 200 OK
                        Date: Thu, 08 Feb 2025 14:00:00 GMT
                        Server: Apache/2.4.46 (Ubuntu)
                        Content-Type: text/html; charset=UTF-8
                        Content-Length: 292
                        Connection: keep-alive
                        Keep-Alive: timeout=5, max=100

                        <!doctype html>
                        <html>
                          <head>
                            <title>This is the title of the webpage!</title>
                          </head>
                          <body>
                            <p>This is an example paragraph. Anything in the <strong>body</strong> tag will appear on the page, just like this <strong>p</strong> tag and its contents.</p>
                          </body>
                        </html>)";
    return (defaultR);
}

std::string	errorResponse(int val){
	std::string num = std::to_string(val);
	std::string response = "HTTP/1.1 " + num + " Not Found\r\n"
                           "Date: Tue, 08 Feb 2025 14:00:00 GMT\r\n"
                           "Server: MySimpleServer/1.0\r\n"
                           "Content-Type: text/html; charset=UTF-8\r\n"
                           "Content-Length: " + std::to_string(errorHtml01.size() + errorHtml02.size()) + "\r\n"
                           "Connection: close\r\n"
                           "\r\n" + errorHtml01 + num + errorHtml02;
	return (response);
}
        