#include <gtest/gtest.h>
#include "../src/Player.hpp"

TEST(PlayerTest, SwitchIsGamingTest)
{
	AIOnly::Player tester("tester", "something.lua", 1);

	tester.switchIsGaming();
	ASSERT_EQ(true, tester.getIsGaming());

	tester.switchIsGaming();
	ASSERT_EQ(false, tester.getIsGaming());
}

TEST(PlayerTest, CountTest)
{
	AIOnly::Player tester("tester", "something.lua", 1);

	tester.countWin();
	ASSERT_EQ(1, tester.getWinCount());

	tester.countLose();
	ASSERT_EQ(1, tester.getLoseCount());
}

TEST(PlayerTest, InitTest)
{
	AIOnly::Player tester("tester", "something.lua", 1);

	ASSERT_STREQ("tester", tester.getName().c_str());
	ASSERT_STREQ("something.lua", tester.getCodePath().c_str());
	ASSERT_EQ(false, tester.getIsGaming());
	ASSERT_EQ(0, tester.getWinCount());
	ASSERT_EQ(0, tester.getLoseCount());
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
