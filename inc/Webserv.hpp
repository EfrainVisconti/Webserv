#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string> 
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <cstring>

/* Sistema */
# include <fcntl.h>

/* STL Containers */
# include <map>
# include <vector>

/* Red */
# include <sys/socket.h>
# include <netinet/in.h>

/* Clases */
# include "Server.hpp"
# include "CgiManager.hpp"
# include "Request.hpp"
# include "Response.hpp"

/* Utils */

#define RED "\033[1;31m"
#define RESET "\033[0m"

#define MAX_CONN 512

#endif