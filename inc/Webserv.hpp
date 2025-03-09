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
# include <csignal>

/* STL Containers */
# include <algorithm>
# include <map>
# include <vector>

/* Red */
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

/* Clases */
# include "ConfigFile.hpp"
# include "ConfigParser.hpp"
# include "Location.hpp"
# include "Logger.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Server.hpp"
# include "ServerManager.hpp"

/* Utils */
std::string statusCodeString(short statusCode);
std::string GetDate();
std::string errorResponse(short val, std::string error_page);
int ft_stoi(std::string str);

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define RESET "\033[0m"

# define MAX_CONN 512
# define DEBUG_MODE 0
# define TIMEOUT 60000 // 60 segs.

# ifndef REQUEST_SIZE
#  define REQUEST_SIZE 100000
# endif

#define MAX_CONTENT_LENGTH 1000000


#endif