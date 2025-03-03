#ifndef SERVER_HPP
#define SERVER_HPP

# include "Webserv.hpp"
# include "Webserv.hpp"


class Location;

#define VALID_LOCATION 			0
#define CGI_ISSUE 				1
#define INVALID_PATH 			2
#define INVALID_REDIRECT_FILE 	3
#define INVALID_ALIAS_FILE 		4
#define INVALID_INDEX_LOCATION 	5

/* Server es la clase que contiene la información de cada servidor
individual */

class Server
{
	private:
		uint32_t						_host;
		uint16_t						_port;
		std::string						_server_name;
		std::string						_root;
		unsigned long					_client_max_body_size;
		std::string						_index;
		std::map<short, std::string>	_error_pages;
		std::vector<Location> 			_locations;
        struct sockaddr_in 				_server_address;
        int     						_listen_socket;
		bool							_autoindex;

	public:
		Server();
		~Server();
		Server(const Server &other);
		Server &operator=(const Server & other);

		void initErrorPages(void);

		void setServerName(std::string server_name);
		void setHost(std::string parametr);
		void setRoot(std::string root);
		void setSocket(int);
		void setPort(std::string parametr);
		void setClientMaxBodySize(std::string parametr);
		void setErrorPages(std::vector<std::string> &parametr);
		void setIndex(std::string index);
		void setLocation(std::string nameLocation, std::vector<std::string> parametr);
		void setAutoindex(std::string autoindex);
		void setServerAddress();
		void setListenSocket(int socket);

		bool isValidHost(std::string host) const;
		bool isValidErrorPages();
		int  isValidLocation(Location &location) const;

		const in_addr_t 						&getHost() const;
		const uint16_t 							&getPort() const;
		const std::string 						&getServerName() const;
		const std::string 						&getRoot() const;
		const size_t 							&getClientMaxBodySize() const;
		const std::string 						&getIndex() const;
		const std::map<short, std::string> 		&getErrorPages() const;
		const std::vector<Location> 			&getLocations() const;
		const struct sockaddr_in 				&getServerAddress() const;
		const int 								&getListenSocket() const;
		const bool 								&getAutoindex() const;

		const std::string 						&getPathErrorPage(short key);
		const std::vector<Location>::iterator 	getLocationKey(std::string key);
		const int								&getSocket() const;

		static void checkToken(std::string &parametr);
		bool 		checkLocations() const;

		public:
		
		class ErrorException : public std::exception
		{
			private:
				std::string _message;
			public:
				ErrorException(std::string message) throw()
				{
					_message = "SERVER CONFIG ERROR: " + message;
				}
				virtual const char* what() const throw()
				{
					return (_message.c_str());
				}
				virtual ~ErrorException() throw() {}
		};

		void	setupServer();
	
};

#endif
