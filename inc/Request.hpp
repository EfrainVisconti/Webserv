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
		std::vector<char>					_body;
		std::string							_content_type;
		short								_error_type;

		Request();
		public:
		Request(unsigned long max_body_size);
		~Request();
		
		short					parseRequest(std::string _request);
		void					parseSetup(std::string _request);
		int						verifyMethod();
		void					printRequestClass();
	// getters
		const std::string& 			getMethod() const;
		const std::string& 			getPath() const;
		const std::string& 			getHost() const;
		const std::string& 			getUserAgent() const;
		const std::string& 			getRequestFormat() const;
		const std::string& 			getLanguage() const;
		const std::string&			getEncoding() const;
		const std::vector<char>&	getBody() const;
		const std::string& 			getContentType() const;
	
		bool					getKeepAliveState() const;
		size_t					getMaxBodySize() const;
		size_t					getBodySize() const;
		short					getErrorType() const;
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
		void					setKeepAlive(bool to);
		void					setContentType(std::string &content_type);
		void					setBody(std::vector<char> &body);
};


#endif