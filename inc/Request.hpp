#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Webserv.hpp"

class Request
{
	private:
		std::string							_method; // metodo (get, post, delete)
		std::string							_path_req; // 
		std::string							_host;
		std::string							_user_agent;
		std::string							_request_format;
		std::string							_language;
		std::string							_encoding;
		bool								_keep_alive;
		size_t								_max_body_size;
		size_t								_body_size;
		int									_error_type;

		Request();
	public:
		Request(int max_body_size);
		~Request();

		void					parseSetup(std::string _request, Request& req);
		void					clean();
		int						verifyMethodHost(std::string host);
	// getters
		const std::string& getMethod() const;
		const std::string& getPath() const;
		const std::string& getHost() const;
		const std::string& getUserAgent() const;
		const std::string& getRequestFormat() const;
		const std::string& getLanguage() const;
		const std::string& getEncoding() const;	
	
		bool					getKeepAliveState() const;
		size_t					getMaxBodySize() const;
		size_t					getBodySize() const;
		int						getErrorType() const;
	// setters
		void					setMethod(std::string &method);
		void					setEncoding(std::string &encoding);
		void					setLanguage(std::string &language);
		void					setReqFormat(std::string &format);
		void					setUserAgent(std::string &useragent);
		void					setPath(std::string &path);
		void					setHost(std::string &host);
		void					setBodySize(size_t size);
		void					setErrorType(int error_type);
		bool					setKeepAlive(int to);
};

void	printRequestClass(const Request &req);

#endif