#ifndef WEBSERV_HPP
# define WEBSERV_HPP

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
# include "CgiManager.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Location.hpp"

/* Utils */

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define RESET "\033[0m"

# define MAX_CONN 512
# define DEBUG_MODE 0
# define TIMEOUT -1 // 60000 // 60 segs.

# define PATH_NOT_FOUND 0
# define PATH_IS_FILE 1
# define PATH_IS_DIRECTORY 2

#endif