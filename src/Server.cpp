#include "../inc/Server.hpp"

/**
 * @brief Default constructor for the Server class.
 *
 * Initializes the member variables with default values and calls initErrorPages() to set up default error pages.
 */
Server::Server()
{
	this->_port = 0;
	this->_host = 0;
	this->_server_name = "";
	this->_root = "";
	this->_client_max_body_size = MAX_CONTENT_LENGTH;
	this->_index = "";
	this->_listen_socket = 0;
	this->_autoindex = false;
	this->initErrorPages();
}

/**
 * @brief Destructor for the Server class.
 *
 * Cleans up any resources allocated by the Server class.
 */
Server::~Server() {}

/**
 * @brief Copy constructor for the Server class.
 *
 * @param other Another instance of the Server class to copy from.
 *
 * This constructor performs a deep copy of all member variables from another Server object.
 */
Server::Server(const Server &other)
{
	if (this != &other)
	{
		this->_server_name = other._server_name;
		this->_root = other._root;
		this->_host = other._host;
		this->_port = other._port;
		this->_client_max_body_size = other._client_max_body_size;
		this->_index = other._index;
		this->_error_pages = other._error_pages;
		this->_locations = other._locations;
		this->_listen_socket = other._listen_socket;
		this->_autoindex = other._autoindex;
		this->_server_address = other._server_address;

	}
	return ;
}

/**
 * @brief Assignment operator for the Server class.
 *
 * @param other Another instance of the Server class to assign to this one.
 *
 * This operator performs a deep copy of all member variables from another Server object.
 *
 * @return A reference to the current Server object.
 */
Server &Server::operator=(const Server &other)
{
    if (this != &other)
    {
		this->_server_name = other._server_name;
		this->_root = other._root;
		this->_port = other._port;
		this->_host = other._host;
		this->_client_max_body_size = other._client_max_body_size;
		this->_index = other._index;
		this->_error_pages = other._error_pages;
		this->_locations = other._locations;
		this->_listen_socket = other._listen_socket;
		this->_autoindex = other._autoindex;
		this->_server_address = other._server_address;
    }
    return *this;
}

/**
 * @brief Initializes the default error pages for common HTTP status codes.
 *
 * This function assigns empty paths to common HTTP error status codes like 301, 302, 404, etc.
 */
void Server::initErrorPages(void)
{
	_error_pages[400] = "";
	_error_pages[403] = "";
	_error_pages[404] = "";
	_error_pages[405] = "";
	_error_pages[413] = "";
	_error_pages[414] = "";
	_error_pages[415] = "";
	_error_pages[500] = "";
	_error_pages[504] = "";
}

/**
 * @brief Sets the server name.
 *
 * @param server_name The name of the server to be set.
 *
 * Throws an exception if the server name contains invalid characters.
 */
void Server::setServerName(std::string server_name)
{
	checkToken(server_name);
	this->_server_name = server_name;
}

/**
 * @brief Sets the host address for the server.
 *
 * @param parametr The host address as a string.
 *
 * Throws an exception if the host address is not valid.
 */
void Server::setHost(std::string parametr)
{
	checkToken(parametr);
	if (parametr == "localhost")
		parametr = "127.0.0.1";
	if (!isValidHost(parametr))
		throw ErrorException("Wrong syntax: host");
	inet_pton(AF_INET, parametr.c_str(), &this->_host);
}

/**
 * @brief Sets the root directory for the server.
 *
 * @param root The root directory path.
 *
 * Throws an exception if the path is not valid or does not exist.
 */
void Server::setRoot(std::string root)
{
	checkToken(root);
	if (root.find_first_not_of("/") == std::string::npos)
		throw Server::ErrorException("Invalid location root: \"/\" or empty");
	if (ConfigFile::getTypePath(root) == 2)
	{
		this->_root = root;
		return ;
	}
	char dir[1024];
	getcwd(dir, 1024);
	std::string full_root = dir + root;
	if (ConfigFile::getTypePath(full_root) != 2)
		throw ErrorException("Wrong syntax: root");
	this->_root = full_root;
}


/**
 * @brief Sets the file descriptor for the server's listen socket.
 *
 * @param fd The file descriptor.
 */
void	Server::setSocket(int fd)
{
	this->_listen_socket = fd;
}

/**
 * @brief Sets the port for the server.
 *
 * @param parametr The port number as a string.
 *
 * Throws an exception if the port is not a valid number or is out of the valid range (1 to 65535).
 */
void Server::setPort(std::string parametr)
{
	unsigned int port;

	port = 0;
	checkToken(parametr);
	for (size_t i = 0; i < parametr.length(); i++)
	{
		if (!std::isdigit(parametr[i]))
			throw ErrorException("Wrong syntax: port");
	}
	port = ft_stoi((parametr));
	if (port < 1 || port > 65636)
		throw ErrorException("Wrong syntax: port");
	this->_port = (uint16_t) port;
}

/**
 * @brief Sets the maximum allowed body size for client requests.
 *
 * @param parametr The body size limit as a string.
 *
 * Throws an exception if the value is invalid.
 */
void Server::setClientMaxBodySize(std::string parametr)
{
	unsigned long body_size;

	body_size = 0;
	checkToken(parametr);
	for (size_t i = 0; i < parametr.length(); i++)
	{
		if (parametr[i] < '0' || parametr[i] > '9')
			throw ErrorException("Wrong syntax: client_max_body_size");
	}
	if (!ft_stoi(parametr))
		throw ErrorException("Wrong syntax: client_max_body_size");
	body_size = ft_stoi(parametr);
	this->_client_max_body_size = body_size;
}

/**
 * @brief Sets the index file for the server.
 *
 * @param index The index file name.
 */
void Server::setIndex(std::string index)
{
	checkToken(index);
	this->_index = index;
}


/**
 * @brief Sets custom error pages for the server.
 *
 * @param parametr A vector of strings containing error codes and their corresponding paths.
 *
 * Throws an exception if the error codes are invalid or if the file paths are inaccessible.
 */
void Server::setErrorPages(std::vector<std::string> &parametr)
{
	if (parametr.empty())
		return;
	if (parametr.size() % 2 != 0)
		throw ErrorException ("Error page initialization faled");
	for (size_t i = 0; i < parametr.size() - 1; i++)
	{
		for (size_t j = 0; j < parametr[i].size(); j++) {
			if (!std::isdigit(parametr[i][j]))
				throw ErrorException("Error code is invalid");
		}
		if (parametr[i].size() != 3)
			throw ErrorException("Error code is invalid");
		short code_error = ft_stoi(parametr[i]);
		if (statusCodeString(code_error)  == "Undefined" || code_error < 400)
			throw ErrorException ("Incorrect error code: " + parametr[i]);
		i++;
		std::string path = parametr[i];
		checkToken(path);
		if (ConfigFile::getTypePath(path) != 2)
		{
			if (ConfigFile::getTypePath(this->_root + path) != 1)
				throw ErrorException ("Incorrect path for error page file: " + this->_root + path);
			if (ConfigFile::checkFile(this->_root + path, 0) == -1 || ConfigFile::checkFile(this->_root + path, 4) == -1)
				throw ErrorException ("Error page file :" + this->_root + path + " is not accessible");
		}
		std::map<short, std::string>::iterator it = this->_error_pages.find(code_error);
		if (it != _error_pages.end())
			this->_error_pages[code_error] = path;
		else
			this->_error_pages.insert(std::make_pair(code_error, path));
	}
}

/**
 * @brief Sets the configuration for a location.
 *
 * This function parses the given parameters and configures the `Location` object with attributes such as root, allowed methods,
 * autoindex, index, return, alias, CGI extensions, CGI path, and client max body size. It also performs validation and
 * handles errors for invalid or duplicate parameters.
 *
 * @param path The path of the location.
 * @param parametr A vector of strings representing the parameters for configuring the location.
 * @throws ErrorException If there are duplicate parameters or invalid tokens.
 */
void Server::setLocation(std::string path, std::vector<std::string> parametr)
{
	Location new_location;
	std::vector<std::string> methods;
	bool flag_methods = false;
	bool flag_autoindex = false;
	bool flag_max_size = false;
	int valid;

	new_location.setPath(path);
	for (size_t i = 0; i < parametr.size(); i++)
	{
		if (parametr[i] == "root" && (i + 1) < parametr.size())
		{
			if (!new_location.getRootLocation().empty())
				throw ErrorException("Root of location is duplicated");
			checkToken(parametr[++i]);
			if (ConfigFile::getTypePath(parametr[i]) == 2)
				new_location.setRootLocation(parametr[i]);
			else
				new_location.setRootLocation(parametr[i]);
		}
		else if ((parametr[i] == "allow_methods" || parametr[i] == "methods") && (i + 1) < parametr.size())
		{
			if (flag_methods)
				throw ErrorException("Allow_methods of location is duplicated");
			std::vector<std::string> methods;
			while (++i < parametr.size())
			{
				if (parametr[i].find(";") != std::string::npos)
				{
					checkToken(parametr[i]);
					methods.push_back(parametr[i]);
					break ;
				}
				else
				{
					methods.push_back(parametr[i]);
					if (i + 1 >= parametr.size())
						throw ErrorException("Token is invalid");
				}
			}
			new_location.setMethods(methods);
			flag_methods = true;
		}
		else if (parametr[i] == "autoindex" && (i + 1) < parametr.size())
		{
			if (path == "/cgi-bin")
				throw ErrorException("Parametr autoindex not allow for CGI");
			if (flag_autoindex)
				throw ErrorException("Autoindex of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setAutoindex(parametr[i]);
			flag_autoindex = true;
		}
		else if (parametr[i] == "index" && (i + 1) < parametr.size())
		{
			if (!new_location.getIndexLocation().empty())
				throw ErrorException("Index of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setIndexLocation(parametr[i]);
		}
		else if (parametr[i] == "return" && (i + 1) < parametr.size())
		{
			if (path == "/cgi-bin")
				throw ErrorException("Parametr return not allow for CGI");
			if (!new_location.getReturn().empty())
				throw ErrorException("Return of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setReturn(parametr[i]);
		}
		else if (parametr[i] == "alias" && (i + 1) < parametr.size())
		{
			if (path == "/cgi-bin")
				throw ErrorException("Parametr alias not allow for CGI");
			if (!new_location.getAlias().empty())
				throw ErrorException("Alias of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setAlias(parametr[i]);
		}
		else if (parametr[i] == "cgi_ext" && (i + 1) < parametr.size())
		{
			std::vector<std::string> extension;
			while (++i < parametr.size())
			{
				if (parametr[i].find(";") != std::string::npos)
				{
					checkToken(parametr[i]);
					extension.push_back(parametr[i]);
					break ;
				}
				else
				{
					extension.push_back(parametr[i]);
					if (i + 1 >= parametr.size())
						throw ErrorException("Token is invalid");
				}
			}
			new_location.setCgiExtension(extension);
		}
		else if (parametr[i] == "cgi_path" && (i + 1) < parametr.size())
		{
			std::vector<std::string> path;
			while (++i < parametr.size())
			{
				if (parametr[i].find(";") != std::string::npos)
				{
					checkToken(parametr[i]);
					path.push_back(parametr[i]);
					break ;
				}
				else
				{
					path.push_back(parametr[i]);
					if (i + 1 >= parametr.size())
						throw ErrorException("Token is invalid");
				}
				if (parametr[i].find("/python") == std::string::npos && parametr[i].find("/bash") == std::string::npos)
					throw ErrorException("cgi_path is invalid");
			}
			new_location.setCgiPath(path);
		}
		else if (parametr[i] == "client_max_body_size" && (i + 1) < parametr.size())
		{
			if (flag_max_size)
				throw ErrorException("Maxbody_size of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setMaxBodySize(parametr[i]);
			flag_max_size = true;
		}
		else if (i < parametr.size())
			throw ErrorException("Parametr in a location is invalid");
	}

	if (!flag_max_size)
		new_location.setMaxBodySize(this->_client_max_body_size);
	valid = isValidLocation(new_location);
	if (valid == 1)
		throw ErrorException("Failed CGI validation");
	else if (valid == 2)
		throw ErrorException("Failed path in location validation");
	this->_locations.push_back(new_location);
}

/**
 * @brief Enables or disables the autoindex feature.
 *
 * @param autoindex The autoindex value as a string ("on" or "off").
 *
 * Throws an exception if the value is invalid.
 */
void Server::setAutoindex(std::string autoindex)
{
	checkToken(autoindex);
	if (autoindex != "on" && autoindex != "off")
		throw ErrorException("Wrong syntax: autoindex");
	if (autoindex == "on")
		this->_autoindex = true;
}


/**
 * @brief Sets the server address based on _host and _port.
 *
 * This method initializes the _server_address structure using the stored
 * values of _host and _port.
 */
void Server::setServerAddress()
{
    std::memset(&_server_address, 0, sizeof(_server_address));
    _server_address.sin_family = AF_INET;
    _server_address.sin_addr.s_addr = _host;
    _server_address.sin_port = htons(_port);
}

/**
 * @brief Sets the listening socket file descriptor.
 *
 * @param socket The file descriptor to be assigned to _listen_socket.
 */
void Server::setListenSocket(int socket)
{
    _listen_socket = socket;
}


/**
 * @brief Validates if the provided host address is valid.
 *
 * @param host The host address as a string.
 *
 * @return True if the host is valid, false otherwise.
 */
bool Server::isValidHost(std::string host) const
{
	struct sockaddr_in sockaddr;
  	return (inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);
}

/**
 * @brief Validates the error pages configuration.
 *
 * This function checks if the configured error pages (HTTP status codes 100 to 599) are valid and if the associated files exist
 * and are accessible.
 *
 * @return True if the error pages configuration is valid, otherwise false.
 */
bool Server::isValidErrorPages()
{
	std::map<short, std::string>::const_iterator it;
	for (it = this->_error_pages.begin(); it != this->_error_pages.end(); it++)
	{
		if (it->first < 100 || it->first > 599)
			return (false);
		if (ConfigFile::checkFile(getRoot() + it->second, 0) < 0 || ConfigFile::checkFile(getRoot() + it->second, 4) < 0)
			return (false);
	}
	return (true);
}

/**
 * @brief Validates the configuration of a location.
 *
 * This function checks the validity of a `Location` object by ensuring it has proper file paths, extensions, and other attributes
 * required for correct configuration. It validates CGI-related settings, file paths, redirect files, alias files, and index location,
 * returning specific error codes for different validation failures.
 *
 * @param location The location to validate.
 * @return An integer representing the validation result:
 *         - `VALID_LOCATION 			(0)` if the location is valid.
 *         - `CGI_ISSUE 				(1)` if there are CGI-related issues, such as missing or invalid CGI paths or extensions.
 *         - `INVALID_PATH 				(2)` if the path is invalid (doesn't start with a '/').
 */
int Server::isValidLocation(Location &location) const
{
	if (location.getPath() == "/cgi-bin")
	{
		if (location.getCgiPath().empty() || location.getCgiExtension().empty() || location.getIndexLocation().empty())
			return CGI_ISSUE;

		if (location.getCgiPath().size() != location.getCgiExtension().size())
			return CGI_ISSUE;
		std::vector<std::string>::const_iterator it;
		for (it = location.getCgiPath().begin(); it != location.getCgiPath().end(); ++it)
		{
			if (ConfigFile::getTypePath(*it) < 0)
				return CGI_ISSUE;
		}
		std::vector<std::string>::const_iterator it_path;
		for (it = location.getCgiExtension().begin(); it != location.getCgiExtension().end(); ++it)
		{
			std::string tmp = *it;
			if (tmp != ".py" && tmp != ".sh" && tmp != "*.py" && tmp != "*.sh")
				return CGI_ISSUE;
			for (it_path = location.getCgiPath().begin(); it_path != location.getCgiPath().end(); ++it_path)
			{
				std::string tmp_path = *it_path;
				if (tmp == ".py" || tmp == "*.py")
				{
					if (tmp_path.find("python") != std::string::npos)
						location._ext_path.insert(std::make_pair(".py", tmp_path));
				}
				else if (tmp == ".sh" || tmp == "*.sh")
				{
					if (tmp_path.find("bash") != std::string::npos)
						location._ext_path[".sh"] = tmp_path;
				}
			}
		}
		if (location.getCgiPath().size() != location.getExtensionPath().size())
			return CGI_ISSUE;
	}
	else
	{
		if (location.getPath()[0] != '/')
			return INVALID_PATH;
		if (location.getRootLocation().empty()) {
			location.setRootLocation("");
		}
	}
	return VALID_LOCATION;
}

/**
 * @brief Returns the host address of the server.
 *
 * @return A constant reference to the server's host address.
 */
const in_addr_t &Server::getHost() const
{
	return (this->_host);
}

/**
 * @brief Returns the server's port number.
 *
 * @return A constant reference to the server's port number.
 */
const uint16_t &Server::getPort() const
{
	return (this->_port);
}

/**
 * @brief Returns the server's name.
 *
 * @return A constant reference to the server name.
 */
const std::string &Server::getServerName() const
{
	return (this->_server_name);
}

/**
 * @brief Returns the root directory of the server.
 *
 * @return A constant reference to the root directory.
 */
const std::string &Server::getRoot() const
{
	return (this->_root);
}

/**
 * @brief Returns the maximum client body size.
 *
 * @return A constant reference to the client max body size.
 */
const size_t &Server::getClientMaxBodySize() const
{
	return (this->_client_max_body_size);
}

/**
 * @brief Returns whether autoindex is enabled.
 *
 * @return A constant reference to the autoindex setting.
 */
const bool &Server::getAutoindex() const
{
	return (this->_autoindex);
}

/**
 * @brief Returns the list of locations configured for the server.
 *
 * @return A constant reference to the vector of locations.
 */
const std::vector<Location> &Server::getLocations() const
{
	return (this->_locations);
}


/**
 * @brief Obtains the server address structure.
 *
 * @return A reference to the sockaddr_in structure containing the server's address.
 */
const struct sockaddr_in &Server::getServerAddress() const
{
    return _server_address;
}

/**
 * @brief Returns the default index file name.
 *
 * @return A constant reference to the index file name.
 */
const std::string &Server::getIndex() const
{
	return (this->_index);
}

/**
 * @brief Returns the error pages mapping (HTTP status code -> file path).
 *
 * @return A constant reference to the map of error pages.
 */
const std::map<short, std::string> &Server::getErrorPages() const
{
	return (this->_error_pages);
}

/**
 * @brief Returns the file descriptor for the server's listen socket
 *
 * @return A constant reference to the file descriptor.
 */
const int   &Server::getSocket() const
{
	return (this->_listen_socket);
}

/**
 * @brief Returns the file descriptor for the server's listen socket
 *
 * @return A constant reference to the file descriptor.
 */
const int   &Server::getListenSocket() const
{
	return (this->_listen_socket);
}


/**
 * @brief Finds the location by its path.
 *
 * This function searches for a location by its path and returns an iterator pointing to it in the list of locations.
 *
 * @param key The path of the location.
 * @return An iterator to the location in the list.
 * @throws ErrorException If no location with the specified path is found.
 */
const std::vector<Location>::iterator Server::getLocationKey(std::string key)
{
	std::vector<Location>::iterator it;
	for (it = this->_locations.begin(); it != this->_locations.end(); it++)
	{
		if (it->getPath() == key)
			return (it);
	}
	throw ErrorException("Error: path to location not found");
}

/**
 * @brief Checks if a parameter token ends correctly.
 *
 * This function checks if a parameter token ends with a semicolon (`;`). If it does not, it throws an exception.
 *
 * @param parametr A string representing the parameter token to check.
 * @throws ErrorException If the token does not end with a semicolon.
 */
void Server::checkToken(std::string &parametr)
{
	size_t pos = parametr.rfind(';');
	if (pos != parametr.size() - 1)
		throw ErrorException("Token is invalid");
	parametr.erase(pos);
}

/**
 * @brief Checks for duplicate locations.
 *
 * This function checks if there are any duplicate locations in the server's location list. If there are duplicates, it returns true.
 *
 * @return True if there are duplicate locations, false otherwise.
 */
bool Server::checkLocations() const
{
	if (this->_locations.size() < 2)
		return (false);
	std::vector<Location>::const_iterator it1;
	std::vector<Location>::const_iterator it2;
	for (it1 = this->_locations.begin(); it1 != this->_locations.end() - 1; it1++) {
		for (it2 = it1 + 1; it2 != this->_locations.end(); it2++) {
			if (it1->getPath() == it2->getPath())
				return (true);
		}
	}
	return (false);
}