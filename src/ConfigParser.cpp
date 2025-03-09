#include "../inc/ConfigParser.hpp"


/**
 * @brief Constructs a ConfigParser object with server vector initialiced.
 */
ConfigParser::ConfigParser(std::vector<Server>& servers)
    : _servers(servers) { }

/**
 * @brief Destroys the ConfigParser object.
 */
ConfigParser::~ConfigParser() { }


/**
 * @brief Checks if a server with the same host, port, and server name already exists.
 * @param host The host to check.
 * @param port The port to check.
 * @param serverName The server name to check.
 * @return True if a server with the same host, port, and server name already exists.
 */
bool ConfigParser::isDuplicateServer(const uint32_t &host, const uint16_t &port)
{
    for (size_t i = 0; i < _servers.size(); i++)
    {
        if (_servers[i].getHost() == host && _servers[i].getPort() == port)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Reads and parses a configuration file, creating a cluster of servers.
 * @param config_file Path to the configuration file.
 * @return Always returns 0.
 * @throws ErrorException If the file is invalid, inaccessible, empty, or has inconsistencies.
 */
int ConfigParser::createCluster(const std::string &config_file)
{
	std::string		content;
	ConfigFile		file(config_file);

	if (file.getTypePath(file.getPath()) != 1)
		throw ErrorException("File is invalid");
	if (file.checkFile(file.getPath(), 4) == -1)
		throw ErrorException("File is not accessible");
	content = file.readFile(config_file);
	if (content.empty())
		throw ErrorException("File is empty");

	removeComments(content);
	removeWhiteSpace(content);
	splitServers(content);

	if (this->_server_config.size() != this->_nb_server)
		throw ErrorException("The server config size != the number of servers");

	for (size_t i = 0; i < this->_nb_server; i++)
	{
		Server server;

		createServer(this->_server_config[i], server);
        if (!isDuplicateServer(server.getHost(), server.getPort()))
        {
            this->_servers.push_back(server);
        }
	}

	if (this->_nb_server > 1)
		checkServers();

	return (0);
}

/**
 * @brief Removes comments from the configuration content.
 * @param content The configuration content as a string.
 */
void ConfigParser::removeComments(std::string &content)
{
	size_t pos;

	pos = content.find('#');
	while (pos != std::string::npos)
	{
		size_t pos_end;
		pos_end = content.find('\n', pos);
		content.erase(pos, pos_end - pos);
		pos = content.find('#');
	}
}

/**
 * @brief Trims whitespace from the beginning and end of the content.
 * @param content The configuration content as a string.
 */
void ConfigParser::removeWhiteSpace(std::string &content)
{
	size_t	i = 0;

	while (content[i] && isspace(content[i]))
		i++;
	content = content.substr(i);
	i = content.length() - 1;
	while (i > 0 && isspace(content[i]))
		i--;
	content = content.substr(0, i + 1);
}

/**
 * @brief Splits the configuration into separate server blocks.
 * @param content The configuration content as a string.
 * @throws ErrorException If no servers are found or there is a scope issue.
 */
void ConfigParser::splitServers(std::string &content)
{
	size_t start = 0;
	size_t end = 1;

	if (content.find("server", 0) == std::string::npos)
		throw ErrorException("Server did not find");
	while (start != end && start < content.length())
	{
		start = findStartServer(start, content);
		end = findEndServer(start, content);
		if (start == end)
			throw ErrorException("problem with scope");
		this->_server_config.push_back(content.substr(start, end - start + 1));
		this->_nb_server++;
		start = end + 1;
	}
}

/**
 * @brief Finds the start of a server block in the configuration.
 *
 * @param start The starting position for searching.
 * @param content The configuration content.
 * @return The index of the opening '{' of the server block.
 * @throws ErrorException if an invalid character is found.
 */
size_t ConfigParser::findStartServer (size_t start, std::string &content)
{
	size_t i;

	for (i = start; content[i]; i++)
	{
		if (content[i] == 's')
			break ;
		if (!isspace(content[i]))
			throw  ErrorException("Wrong character out of server scope{}");
	}
	if (!content[i])
		return (start);
	if (content.compare(i, 6, "server") != 0)
		throw ErrorException("Wrong character out of server scope{}");
	i += 6;
	while (content[i] && isspace(content[i]))
		i++;
	if (content[i] == '{')
		return (i);
	else
		throw  ErrorException("Wrong character out of server scope{}");

}

/**
 * @brief Finds the end of a server block in the configuration.
 *
 * @param start The starting position for searching.
 * @param content The configuration content.
 * @return The index of the closing '}' of the server block.
 */
size_t ConfigParser::findEndServer (size_t start, std::string &content)
{
	size_t	i;
	size_t	scope;

	scope = 0;
	for (i = start + 1; content[i]; i++)
	{
		if (content[i] == '{')
			scope++;
		if (content[i] == '}')
		{
			if (!scope)
				return (i);
			scope--;
		}
	}
	return (start);
}

/**
 * @brief Splits a line into parameters based on a separator.
 *
 * @param line The input string to be split.
 * @param sep The separator characters.
 * @return A vector of substrings.
 */
std::vector<std::string> splitParametrs(std::string line, std::string sep)
{
	std::vector<std::string>	str;
	std::string::size_type		start, end;

	start = end = 0;
	while (1)
	{
		end = line.find_first_of(sep, start);
		if (end == std::string::npos)
			break;
		std::string tmp = line.substr(start, end - start);
		str.push_back(tmp);
		start = line.find_first_not_of(sep, end);
		if (start == std::string::npos)
			break;
	}
	return (str);
}

/**
 * @brief Creates a Server object from a configuration string.
 *
 * @param config The configuration string.
 * @param server The Server object to populate.
 * @throws ErrorException if the configuration is invalid.
 */
void ConfigParser::createServer(std::string &config, Server &server)
{
	std::vector<std::string>	parametrs;
	std::vector<std::string>	error_codes;
	int		flag_loc = 1;
	bool	flag_autoindex = false;
	bool	flag_max_size = false;

	parametrs = splitParametrs(config += ' ', std::string(" \n\t"));
	if (parametrs.size() < 3)
		throw  ErrorException("Failed server validation");
	for (size_t i = 0; i < parametrs.size(); i++)
	{
		if (parametrs[i] == "listen" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (server.getPort())
				throw  ErrorException("Port is duplicated");
			server.setPort(parametrs[++i]);
		}
		else if (parametrs[i] == "location" && (i + 1) < parametrs.size())
		{
			std::string	path;
			i++;
			if (parametrs[i] == "{" || parametrs[i] == "}")
				throw  ErrorException("Wrong character in server scope{}");
			path = parametrs[i];
			std::vector<std::string> codes;
			if (parametrs[++i] != "{")
				throw  ErrorException("Wrong character in server scope{}");
			i++;
			while (i < parametrs.size() && parametrs[i] != "}")
				codes.push_back(parametrs[i++]);
			server.setLocation(path, codes);
			if (i < parametrs.size() && parametrs[i] != "}")
				throw  ErrorException("Wrong character in server scope{}");
			flag_loc = 0;
		}
		else if (parametrs[i] == "host" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (server.getHost())
				throw  ErrorException("Host is duplicated");
			server.setHost(parametrs[++i]);
		}
		else if (parametrs[i] == "root" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (!server.getRoot().empty())
				throw  ErrorException("Root is duplicated");
			server.setRoot(parametrs[++i]);
		}
		else if (parametrs[i] == "error_page" && (i + 1) < parametrs.size() && flag_loc)
		{
			while (++i < parametrs.size())
			{
				error_codes.push_back(parametrs[i]);
				if (parametrs[i].find(';') != std::string::npos)
					break ;
				if (i + 1 >= parametrs.size())
					throw ErrorException("Wrong character out of server scope{}");
			}
		}
		else if (parametrs[i] == "client_max_body_size" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (flag_max_size)
				throw  ErrorException("Client_max_body_size is duplicated");
			server.setClientMaxBodySize(parametrs[++i]);
			flag_max_size = true;
		}
		else if (parametrs[i] == "server_name" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (!server.getServerName().empty())
				throw  ErrorException("Server_name is duplicated");
			server.setServerName(parametrs[++i]);
		}
		else if (parametrs[i] == "index" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (!server.getIndex().empty())
				throw  ErrorException("Index is duplicated");
			server.setIndex(parametrs[++i]);
		}
		else if (parametrs[i] == "autoindex" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (flag_autoindex)
				throw ErrorException("Autoindex of server is duplicated");
			server.setAutoindex(parametrs[++i]);
			flag_autoindex = true;
		}
		else if (parametrs[i] != "}" && parametrs[i] != "{")
		{
			if (!flag_loc)
				throw  ErrorException("Parametrs after location");
			else
				throw  ErrorException("Unsupported directive");
		}
	}
	if (server.getRoot().empty())
		server.setRoot(";");
	if (server.getHost() == 0)
		server.setHost("localhost;");
	if (server.getIndex().empty())
		server.setIndex(";");
	if (server.checkLocations())
		throw ErrorException("Location is duplicated");
	if (!server.getPort())
		throw ErrorException("Port not found");
	server.setErrorPages(error_codes);
	if (!server.isValidErrorPages())
		throw ErrorException("Incorrect path for error page or number of error");
}

/**
 * @brief Compares a substring of str1 with str2.
 *
 * @param str1 The first string.
 * @param str2 The second string.
 * @param pos The starting position in str1.
 * @return 0 if the strings match, 1 otherwise.
 */
int	ConfigParser::stringCompare(std::string str1, std::string str2, size_t pos)
{
	size_t	i;

	i = 0;
	while (pos < str1.length() && i < str2.length() && str1[pos] == str2[i])
	{
		pos++;
		i++;
	}
	if (i == str2.length() && pos <= str1.length() && (str1.length() == pos || isspace(str1[pos])))
		return (0);
	return (1);
}

/**
 * @brief Checks for duplicate and mandatory server parameters.
 *
 * @throws ErrorException if there are duplicate servers.
 */
void ConfigParser::checkServers()
{
	std::vector<Server>::iterator it1;
	std::vector<Server>::iterator it2;

	for (it1 = this->_servers.begin(); it1 != this->_servers.end() - 1; it1++)
	{
		for (it2 = it1 + 1; it2 != this->_servers.end(); it2++)
		{
			if (it1->getPort() == it2->getPort() && it1->getHost() == it2->getHost() && it1->getServerName() == it2->getServerName())
				throw ErrorException("Failed server validation");
		}
	}
}

/**
 * @brief Retrieves the list of servers parsed from the configuration.
 *
 * @return A vector containing Server objects.
 */
std::vector<Server>	&ConfigParser::getServers()
{
	return (this->_servers);
}