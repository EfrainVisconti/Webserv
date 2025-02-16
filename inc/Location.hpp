#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Webserv.hpp"

class Location
{
	public:

		std::string			path;
		std::string			root;
		bool				autoindex;
		std::string			index;
		std::vector<short>	methods;
		std::string			redirection;

		Location();
		~Location();

};

#endif