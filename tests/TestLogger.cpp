/*
 * TestLogger.cpp
 *
 *  Created on: 2013. 12. 6.
 *      Author: hibm98
 */

#include <gtest/gtest.h>
#include <fstream>
#include "src/Logger.hpp"

const char* logFileName = "/tmp/test1.log";

TEST(LoggerTest, FileLogger)
{
	// Create Logger
	{
		AIOnly::Logger logger(logFileName);
	}
	// Check to exist log file.
	std::ifstream log_stream(logFileName, std::ios::in);
	ASSERT_TRUE(log_stream);
}

TEST(LoggerTest, StringLogger)
{
	// Create Logger
	AIOnly::Logger logger;
	ASSERT_STREQ("", logger.getLog().c_str());
}

TEST(LoggerTest, Logging)
{
	AIOnly::Logger logger;

	logger.write(AIOnly::LogLevel::INFO, "Test");
	std::string log = logger.getLog();
	ASSERT_TRUE(log.find("Test") != log.npos);
}

TEST(LoggerTest, WriteLog)
{
	{
		AIOnly::Logger logger(logFileName);
		logger.write(AIOnly::LogLevel::INFO, "Test");
	}

	std::ifstream log_stream(logFileName, std::ios::in);
	std::string date, time, level, context;
	log_stream >> date >> time >> level >> context;
	ASSERT_STREQ("Test", context.c_str());
}

TEST(LoggerTest, OpenFileAfterLogWrite)
{
	{
		AIOnly::Logger logger;
		logger.write(AIOnly::LogLevel::INFO, "Test");
		logger.openFile(logFileName, true);
	}

	std::ifstream log_stream(logFileName, std::ios::in);
	std::string date, time, level, context;
	log_stream >> date >> time >> level >> context;
	ASSERT_STREQ("Test", context.c_str());
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
