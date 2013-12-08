/*
 * Logger.hpp
 *
 *  Created on: 2013. 12. 6.
 *      Author: hibm98
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <ctime>

namespace AIOnly
{

enum class LogLevel { ERR, WARN, INFO, DEBUG };

/*
 *
 */
class Logger
{
	std::ofstream 	_logfile;
	std::string		_log;
	bool 			_print_log;

	std::string getTimeString() const;
	inline std::string getFormattedLog(LogLevel level, const std::string& context) const;

public:

	Logger(bool print_log = false);
	Logger(const char* fileName, bool print_log = false);
	virtual ~Logger();

	bool openFile(const std::string& fileName, bool copy_current_log = false);
	inline void closeFile();

	void write(LogLevel level, const std::string& context);
	inline void clear();

	const std::string& getLog() const;
};

} /* namespace AIOnly */

#endif /* LOGGER_HPP_ */
