#include "../inc/ConfigFile.hpp"

/**
* @brief Default constructor.
*/
ConfigFile::ConfigFile() : _size(0) { }

/**
 * @brief Constructs a ConfigFile with a given path.
 * @param path Path to the configuration file.
 */
ConfigFile::ConfigFile(std::string const path) : _path(path), _size(0) { }

/**
 * @brief Destructor.
 */
ConfigFile::~ConfigFile() { }

/**
 * @brief Determines the type of a given path.
 * @param path The path to check.
 * @return PATH_TYPE_FILE if it's a file, PATH_TYPE_FOLDER if it's a directory, PATH_TYPE_OTHER if it's neither, PATH_TYPE_ERROR if it does not exist.
 */
int ConfigFile::getTypePath(std::string const path)
{
	struct stat	buffer;
	int			result;
	
	result = stat(path.c_str(), &buffer);
	if (result == 0)
	{
		if (buffer.st_mode & S_IFREG)
			return PATH_TYPE_FILE;
		else if (buffer.st_mode & S_IFDIR)
			return PATH_TYPE_FOLDER;
		else
			return PATH_TYPE_OTHER;
	}
	else
		return PATH_TYPE_ERROR;
}

/**
 * @brief Checks if the file exists and is accessible.
 * @param path Path to the file.
 * @param mode Access mode (e.g., R_OK for read access).
 * @return 0 if accessible, -1 otherwise.
 */
int	ConfigFile::checkFile(std::string const path, int mode)
{
	return (access(path.c_str(), mode));
}

/**
 * @brief Checks if a file exists and is readable.
 * @param path Base path.
 * @param index File name or relative path.
 * @return 0 if the file exists and is readable, -1 otherwise.
 */
int ConfigFile::isFileExistAndReadable(std::string const path, std::string const index)
{
	if (getTypePath(index) == 1 && checkFile(index, 4) == 0)
		return 0;
	if (getTypePath(path + index) == 1 && checkFile(path + index, 4) == 0)
		return 0;
	return -1;
}

/**
 * @brief Reads the content of a file into a string.
 * @param path Path to the file.
 * @return The file content as a string, or an empty string if reading fails.
 */
std::string	ConfigFile::readFile(std::string path)
{
	if (path.empty() || path.length() == 0)
		return "";
	
	std::ifstream config_file(path.c_str());
	
	if (!config_file || !config_file.is_open())
		return "";

	std::stringstream stream_binding;

	stream_binding << config_file.rdbuf();
	return (stream_binding.str());
}

/**
 * @brief Gets the path of the configuration file.
 * @return The file path.
 */
std::string ConfigFile::getPath()
{
	return (this->_path);
}

/**
 * @brief Gets the size attribute (currently unused).
 * @return The size value.
 */
int ConfigFile::getSize()
{
	return (this->_size);
}