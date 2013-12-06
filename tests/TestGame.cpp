/*
 * TestGame.cpp
 *
 *  Created on: 2013. 11. 3.
 *      Author: hibm98
 */

#include <gtest/gtest.h>
#include "src/Game.hpp"
#include "src/PlayerList.hpp"

class GamePlayTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		test_players = std::make_shared<AIOnly::PlayerList>();
		test_players->insertPlayer(std::make_shared<AIOnly::Player>("Tester1", "tests/Tester1.lua", 1));
		test_players->insertPlayer(std::make_shared<AIOnly::Player>("Tester2", "tests/Tester1.lua", 2));
	}

	AIOnly::PlayerListPtr test_players;
};

TEST_F(GamePlayTest, OneGame)
{
	AIOnly::Game g(test_players);
	g.run();
}


int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
