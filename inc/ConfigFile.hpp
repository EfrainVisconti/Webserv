#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "Webserv.hpp"

/** 
 * @brief Define the possible types of a path.
 */
#define PATH_TYPE_FILE   1	///< Path is a regular file.
#define PATH_TYPE_FOLDER 2	///< Path is a directory.
#define PATH_TYPE_OTHER  3	///< Path exists but is neither a file nor a folder.
#define PATH_TYPE_ERROR  -1	///< Path does not exist or cannot be accessed.


class ConfigFile {
	private:
		std::string		_path;	///< Path to the configuration file.
		size_t			_size;	///< Unused member, consider removing.

	public:

		ConfigFile();
		ConfigFile(std::string const path);
		~ConfigFile();

		static int getTypePath(std::string const path);
		static int checkFile(std::string const path, int mode);
		std::string	readFile(std::string path);
		static int isFileExistAndReadable(std::string const path, std::string const index);

		std::string getPath();
		int getSize();
};

#endif