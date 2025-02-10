#include "../inc/Webserv.hpp"
#include <cstdlib>

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
