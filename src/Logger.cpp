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
}

Logger::~Logger()
{
}

} /* namespace AIOnly */
