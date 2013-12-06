/*
 * TestLogger.cpp
 *
 *  Created on: 2013. 12. 6.
 *      Author: hibm98
 */

#include <gtest/gtest.h>
#include <fstream>
#include "src/Logger.hpp"

TEST(LoggerTest, CreateLogger)
{
	const char* logFileName = "tests/test1.log";

	// Create Logger
	{
		AIOnly::Logger logger(logFileName);
	}
	// Check to exist log file.
	std::ifstream log_stream(logFileName, std::ios::in);
	ASSERT_TRUE(log_stream);
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
