#include "../inc/Location.hpp"

/**
 * @brief Default constructor for the Location class.
 *
 * Initializes all member variables to their default values. The methods vector
 * is reserved for 3 elements, and the default values for the methods are set
 * to 0 (inactive).
 */

Location::Location()
{
	this->_path = "";
	this->_root = "";
	this->_autoindex = false;
	this->_index = "";
	this->_return = "";
	this->_alias = "";
	this->_client_max_body_size = MAX_CONTENT_LENGTH;
	this->_methods.reserve(3);
	this->_methods.push_back("GET");
	this->_methods.push_back("");
	this->_methods.push_back("");
}

/**
 * @brief Copy constructor for the Location class.
 *
 * @param other The Location object to copy from.
 * Copies all member variables from the other object to this one.
 */
Location::Location(const Location &other)
{
	this->_path = other._path;
	this->_root = other._root;
	this->_autoindex = other._autoindex;
	this->_index = other._index;
	this->_cgi_path = other._cgi_path;
	this->_cgi_ext = other._cgi_ext;
	this->_return = other._return;
	this->_alias = other._alias;
    this->_methods = other._methods;
	this->_ext_path = other._ext_path;
	this->_client_max_body_size = other._client_max_body_size;
}

/**
 * @brief Assignment operator for the Location class.
 *
 * @param other The Location object to assign to this one.
 *
 * @return A reference to this Location object.
 * Assigns all member variables from other to this object if they are not the same
 * object.
 */
Location &Location::operator=(const Location &other)
{
	if (this != &other)
	{
		this->_path = other._path;
		this->_root = other._root;
		this->_autoindex = other._autoindex;
		this->_index = other._index;
		this->_cgi_path = other._cgi_path;
		this->_cgi_ext = other._cgi_ext;
		this->_return = other._return;
		this->_alias = other._alias;
		this->_methods = other._methods;
		this->_ext_path = other._ext_path;
		this->_client_max_body_size = other._client_max_body_size;
    }
	return (*this);
}

/**
 * @brief Destructor for the Location class.
 *
 * The destructor cleans up any resources used by the Location object.
 */
Location::~Location() { }

/**
 * @brief Sets the path for the location.
 *
 * @param parametr The path to set.
 */
void Location::setPath(std::string parametr)
{
	this->_path = parametr;
}

/**
 * @brief Sets the root location for the location.
 *
 * @param parametr The root location to set.
 *
 * @throws Server::ErrorException If the path type is invalid.
 */
void Location::setRootLocation(std::string parametr)
{
	if (parametr.empty())
	{
		this->_root = "";
		return;
	}
	if (parametr.find_first_not_of("/") == std::string::npos)
		throw Server::ErrorException("Invalid location root: \"/\"");
	if (ConfigFile::getTypePath(parametr) != 2)
	 	throw Server::ErrorException("root of location");
	if (parametr[parametr.length() - 1] == '/')
		parametr = parametr.substr(0, parametr.length() - 1);
	this->_root = parametr;
}

/**
 * @brief Sets the allowed HTTP methods for this location.
 *
 * @param methods A vector of strings representing HTTP methods (e.g., "GET", "POST").
 *
 * @throws Server::ErrorException If any method is not supported.
 */
void Location::setMethods(std::vector<std::string> methods)
{
    this->_methods.clear();

    for (size_t i = 0; i < methods.size(); i++)
    {
        if (methods[i] == "GET" || methods[i] == "POST" || methods[i] == "DELETE")
        {
            this->_methods.push_back(methods[i]);
        }
        else
        {
            throw Server::ErrorException("Allow method not supported " + methods[i]);
        }
    }
}

/**
 * @brief Sets the autoindex setting for the location.
 *
 * @param parametr A string representing the autoindex setting ("on" or "off").
 *
 * @throws Server::ErrorException If the autoindex setting is invalid.
 */
void Location::setAutoindex(std::string parametr)
{
	if (parametr == "on" || parametr == "off")
		this->_autoindex = (parametr == "on");
	else
		throw Server::ErrorException("Wrong autoindex");
}

/**
 * @brief Sets the index location for the location.
 *
 * @param parametr The index location to set.
 */
void Location::setIndexLocation(std::string parametr)
{
	this->_index = parametr;
}

/**
 * @brief Sets the return URL for the location.
 *
 * @param parametr The return URL to set.
 */
void Location::setReturn(std::string parametr)
{
	this->_return = parametr;
}

/**
 * @brief Sets the alias for the location.
 *
 * @param parametr The alias to set.
 */
void Location::setAlias(std::string parametr)
{
	this->_alias = parametr;
}

/**
 * @brief Sets the CGI paths for the location.
 *
 * @param path A vector of strings representing the CGI paths.
 */
void Location::setCgiPath(std::vector<std::string> path)
{
	this->_cgi_path = path;
}

/**
 * @brief Sets the CGI extensions for the location.
 *
 * @param extension A vector of strings representing the CGI extensions.
 */
void Location::setCgiExtension(std::vector<std::string> extension)
{
	this->_cgi_ext = extension;
}

/**
 * @brief Sets the maximum body size for client requests.
 *
 * @param parametr A string representing the maximum body size.
 *
 * @throws Server::ErrorException If the syntax is incorrect or the value is invalid.
 */
void Location::setMaxBodySize(std::string parametr)
{
	unsigned long body_size = 0;

	for (size_t i = 0; i < parametr.length(); i++)
	{
		if (parametr[i] < '0' || parametr[i] > '9')
			throw Server::ErrorException("Wrong syntax: client_max_body_size");
	}
	if (!ft_stoi(parametr))
		throw Server::ErrorException("Wrong syntax: client_max_body_size");
	body_size = ft_stoi(parametr);
	this->_client_max_body_size = body_size;
}

/**
 * @brief Sets the maximum body size for client requests.
 *
 * @param parametr The maximum body size in bytes.
 */
void Location::setMaxBodySize(unsigned long parametr)
{
	this->_client_max_body_size = parametr;
}

/**
 * @brief Gets the path of the location.
 *
 * @return A constant reference to the path string.
 */
const std::string &Location::getPath() const
{
	return (this->_path);
}

/**
 * @brief Gets the root location of the location.
 *
 * @return A constant reference to the root location string.
 */
const std::string &Location::getRootLocation() const
{
	return (this->_root);
}

/**
 * @brief Gets the index location of the location.
 *
 * @return A constant reference to the index location string.
 */
const std::string &Location::getIndexLocation() const
{
	return (this->_index);
}

/**
 * @brief Gets the allowed HTTP methods for the location.
 *
 * @return A constant reference to the methods vector.
 */
const std::vector<std::string> &Location::getMethods() const
{
	return (this->_methods);
}

/**
 * @brief Gets the CGI paths for the location.
 *
 * @return A constant reference to the CGI paths vector.
 */
const std::vector<std::string> &Location::getCgiPath() const
{
	return (this->_cgi_path);
}

/**
 * @brief Gets the CGI extensions for the location.
 *
 * @return A constant reference to the CGI extensions vector.
 */
const std::vector<std::string> &Location::getCgiExtension() const
{
	return (this->_cgi_ext);
}

/**
 * @brief Gets the autoindex setting for the location.
 *
 * @return A constant reference to the autoindex boolean.
 */
const bool &Location::getAutoindex() const
{
	return (this->_autoindex);
}

/**
 * @brief Gets the return URL for the location.
 *
 * @return A constant reference to the return URL string.
 */
const std::string &Location::getReturn() const
{
	return (this->_return);
}


/**
 * @brief Gets the alias for the location.
 *
 * @return A constant reference to the alias string.
 */
const std::string &Location::getAlias() const
{
	return (this->_alias);
}

/**
 * @brief Gets the extension paths for the location.
 *
 * @return A constant reference to the extension paths map.
 */
const std::map<std::string, std::string> &Location::getExtensionPath() const
{
	return (this->_ext_path);
}

/**
 * @brief Gets the maximum body size for client requests.
 *
 * @return A constant reference to the maximum body size in bytes.
 */
const unsigned long &Location::getMaxBodySize() const
{
	return (this->_client_max_body_size);
}

/**
 * @brief Returns a string representation of the allowed HTTP methods.
 *
 * @return A string with the allowed methods (e.g., "GET, POST").
 */
std::string Location::getPrintMethods() const
{
	std::string res;
	if (!_methods[2].empty())
		res.insert(0, "DELETE");
	if (!_methods[1].empty())
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "POST");
	}
	if (!_methods[0].empty())
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "GET");
	}
	return (res);
}