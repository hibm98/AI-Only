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
#include <fstream>
#include <sstream>

namespace AIOnly
{

/*
 *
 */
class Logger
{
	std::fstream 		_logfile;
	std::stringstream	_log;

	bool _print_log;

public:
	Logger(bool print_log = false);
	Logger(const char* fileName, bool print_log = false);
	virtual ~Logger();
};

} /* namespace AIOnly */

#endif /* LOGGER_HPP_ */
