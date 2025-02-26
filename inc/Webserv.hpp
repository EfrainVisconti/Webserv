#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <fstream>
# include <string>
# include <iostream>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <cstring>
# include <poll.h>
# include <fcntl.h>
# include <sstream>
# include <cstdlib>
# include <sys/stat.h>
# include <dirent.h>
# include <cerrno>

/* STL Containers */
# include <algorithm>
# include <map>
# include <vector>

/* Red */
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

/* Clases */
# include "Server.hpp"
# include "ServerManager.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Location.hpp"

/* Utils */
std::string GetDate();
std::string errorResponse(short val, std::string error_page);

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define RESET "\033[0m"

# define MAX_CONN 512
# define REQUEST_SIZE 8192
# define DEBUG_MODE 1
# define TIMEOUT 60000 // 60 segs.

#endif