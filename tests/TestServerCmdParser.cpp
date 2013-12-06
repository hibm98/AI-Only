/*
 * TestCmdParser.cpp
 *
 *  Created on: 2013. 11. 17.
 *      Author: hibm98
 */

#include "../src/ServerCmdParser.hpp"
#include <gtest/gtest.h>

using namespace AIOnly;

TEST(ParseTest, ParseExitWithoutArgs)
{
	ServerCmdParser parser;

	parser.parse("exit");
	ASSERT_EQ(parser.getCmdType(), ServerCmdType::CLI_EXIT);
	ASSERT_EQ(0, parser.getArgSize());
	ASSERT_TRUE(parser.getArgs().empty());
}

TEST(ParseTest, ParseSayWithArgs)
{
	ServerCmdParser parser;

	parser.parse("say hello");
	ASSERT_EQ(parser.getCmdType(), ServerCmdType::SAY);
	ASSERT_EQ(1, parser.getArgSize());
	ASSERT_STREQ(parser.getArg(0).c_str(), "hello");
}

TEST(ParseTest, ParseCreatePlayerWithAllArgs)
{
	ServerCmdParser parser;

	parser.parse("create_player Tester1 tests/Tester1.lua");
	ASSERT_EQ(parser.getCmdType(), ServerCmdType::CREATE_PLAYER);
	ASSERT_EQ(2, parser.getArgSize());
	ASSERT_STREQ(parser.getArg(0).c_str(), "Tester1");
	ASSERT_STREQ(parser.getArg(1).c_str(), "tests/Tester1.lua");
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
