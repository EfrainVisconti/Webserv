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

/* STL Containers */
# include <map>
# include <vector>

/* Red */
# include <sys/socket.h>
# include <netinet/in.h>

/* Clases */
# include "Server.hpp"
# include "ServerManager.hpp"
# include "CgiManager.hpp"
# include "Request.hpp"
# include "Response.hpp"

/* Utils */

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define RESET "\033[0m"

#define MAX_CONN 512
#define DEBUG_MODE 1
#define TIMEOUT 60000 // 60 segs.

#endif