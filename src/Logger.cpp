/*
 * Logger.cpp
 *
 *  Created on: 2013. 12. 6.
 *      Author: hibm98
 */

#include "Logger.hpp"

namespace AIOnly
{

// 오직 로그만 저장하는 로거를 만드는 생성자.
Logger::Logger(bool print_log)
	: _print_log(print_log)
{
}

// 로그를 파일로 저장해주는 로거를 만드는 생성자.
Logger::Logger(const char* fileName, bool print_log)
	: _print_log(print_log)
{
	_logfile.open(fileName);
}

Logger::~Logger()
{
	this->closeFile();
}

std::string Logger::getTimeString() const
{
	std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char now_time[100];

	std::strftime(now_time, 100, "%Y.%m.%d %H:%M:%S", std::localtime(&t));
	return std::string(now_time);
}

std::string Logger::getFormattedLog(LogLevel level, const std::string& context) const
{
	std::string result = getTimeString().append("\t");

	switch(level)
	{
	case LogLevel::ERR:
		result.append("[ERROR]");
		break;
	case LogLevel::WARN:
		result.append("[WARNING]");
		break;
	case LogLevel::DEBUG:
		result.append("[DEBUG]");
		break;
	case LogLevel::INFO:
	default:
		result.append("[INFO]");
		break;
	}

	return result.append(" ").append(context);
}

void Logger::write(LogLevel level, const std::string& context)
{
	std::string log = this->getFormattedLog(level, context);

	_log.append(log).append("\n");

	if (_print_log)
		std::cout << log << std::endl;

	if (_logfile.is_open())
		_logfile << log << std::endl;
}

bool Logger::openFile(const std::string& fileName, bool copy_current_log)
{
	if (_logfile.is_open())
		_logfile.close();

	_logfile.open(fileName);

	if (copy_current_log && _logfile.is_open())
		_logfile << _log;

	return _logfile.is_open();
}

void Logger::closeFile()
{
	if (_logfile.is_open())
		_logfile.close();
}

void Logger::clear()
{
	_log.clear();
}

const std::string& Logger::getLog() const
{
	return _log;
}

void Logger::echoLog(bool echoable)
{
	_print_log = echoable;
}

} /* namespace AIOnly */
