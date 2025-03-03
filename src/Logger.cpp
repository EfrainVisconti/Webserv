#include "../inc/Logger.hpp"
#include <cstdarg>

std::string Logger::file_name = "logfile.txt";
LogPrio Logger::prio = ERROR;
L_State Logger::state = ON;

/**
 * @brief Static map that associates log priority levels with string representations.
 * 
 * This map stores the string corresponding to each log priority level for easy display.
 */
std::map<LogPrio, std::string> Logger::prio_str = initMap();

/**
 * @brief Initializes the map that associates log priorities with their respective strings.
 * 
 * The map is used to store string representations of various logging priorities.
 * 
 * @return A map where each LogPrio is mapped to its corresponding string representation.
 */
std::map<LogPrio, std::string> Logger::initMap()
{
    std::map<LogPrio, std::string> p_map;

    // p_map[DEBUG] = "[DEBUG]   ";
    p_map[DEBUG] = "[INFO]    ";
    p_map[INFO] = "[DEBUG]    ";
    p_map[ERROR] = "[ERROR]   ";
    return p_map;
}

/**
 * @brief Logs a formatted message to either the console or a file.
 * 
 * This function handles logging the provided message in the specified mode (console or file).
 * It uses a variable argument list to format the message before logging.
 * 
 * @param color The color code used for console output (not used in the current code).
 * @param m The logging mode, either FILE_OUTPUT or CONSOLE_OUTPUT.
 * @param msg The message format string.
 * @param ... The variable arguments for the message formatting.
 */
void    Logger::logMsg(const char *color, Mode m, const char* msg, ...)
{
    char        output[8192];
    va_list     args;
    int         n;

    if (state == ON)
    {
        va_start(args, msg);
        n = vsnprintf(output, 8192, msg, args);
        std::string date = getCurrTime();
        if (m == FILE_OUTPUT)
        {
            if (mkdir("logs", 0777) < 0 && errno != EEXIST)
            {
                std::cerr << "mkdir() Error: " << strerror(errno) << std::endl;
                return ;
            }
            int fd = open(("logs/" + file_name).c_str(), O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR);
            std::cout << "fd is " << fd << "And errno is :" << strerror(errno) << std::endl;
            write(fd, date.c_str(), date.length());
            write(fd, "   ", 3);
            write(fd, output, n);
            write(fd, "\n", 1);
            close(fd);
        }
        else if (m == CONSOLE_OUTPUT)
        {
            std::cout << color << getCurrTime() << output << RESET << std::endl;
        }      
        va_end(args);
    }
}

/**
 * @brief Retrieves the current date and time as a formatted string.
 * 
 * This function fetches the current date and time and returns it as a string in the format
 * "[YYYY-MM-DD HH:MM:SS]   ".
 * 
 * @return A string representing the current date and time.
 */
std::string Logger::getCurrTime()
{
    tzset();
    char date[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    tm.tm_hour = tm.tm_hour + GST;
    strftime(date, sizeof(date), "[%Y-%m-%d  %H:%M:%S]   ", &tm);
    return (std::string(date));
}

/**
 * @brief Sets the logging priority level.
 * 
 * This function allows the user to set the logging priority for the logger.
 * 
 * @param p The new log priority to set.
 */
void Logger::setPrio(LogPrio p)
{
    Logger::prio = p;
}

/**
 * @brief Sets the log file name.
 * 
 * This function allows the user to change the name of the file where the log messages
 * will be written.
 * 
 * @param name The new log file name.
 */
void Logger::setFileName(std::string name)
{
    Logger::file_name = name;
}

/**
 * @brief Sets the logging state.
 * 
 * This function allows the user to enable or disable logging.
 * 
 * @param s The new state to set (ON or OFF).
 */
void Logger::setState(L_State s)
{
    Logger::state = s;
}


