/*
 * TestPlayerList.cpp
 *
 *  Created on: 2013. 11. 14.
 *      Author: hibm98
 */

#include "../src/PlayerList.hpp"
#include <memory>
#include <gtest/gtest.h>

TEST(PlayerInsertTest, Insert)
{
	AIOnly::PlayerList test_players;
	int result;

	// try create player
	result = test_players.insertPlayer(std::make_shared<AIOnly::Player>("Tester1", "tests/user_Tester1_script.lua", 1));
	ASSERT_TRUE(result);

	result = test_players.insertPlayer(std::make_shared<AIOnly::Player>("Tester2", "tests/user_Tester2_script.lua", 2));
	ASSERT_TRUE(result);

	// try create duplicate player
	result = test_players.insertPlayer(std::make_shared<AIOnly::Player>("Tester1", "tests/user_Tester2_script.lua", 1));
	ASSERT_FALSE(result);

	result = test_players.insertPlayer(std::make_shared<AIOnly::Player>("Tester2", "tests/user_Tester2_script.lua", 1));
	ASSERT_FALSE(result);

	result = test_players.insertPlayer(std::make_shared<AIOnly::Player>("Tester1", "tests/user_Tester2_script.lua", 3));
	ASSERT_FALSE(result);
}

class PlayerListTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		test_players.insertPlayer(std::make_shared<AIOnly::Player>("Tester1", "tests/user_Tester1_script.lua", 1));
		test_players.insertPlayer(std::make_shared<AIOnly::Player>("Tester2", "tests/user_Tester2_script.lua", 2));
	}

	AIOnly::PlayerList test_players;
};

// 존재하는 플레이어를 이름으로 찾기.
TEST_F(PlayerListTest, SearchExistPlayerByName)
{
	ASSERT_TRUE(test_players.searchPlayer("Tester2"));
}

// 존재하는 플레이어를 uID로 찾기.
TEST_F(PlayerListTest, SearchExistPlayerById)
{
	ASSERT_TRUE(test_players.searchPlayer(2));
}

// 존재하지 않는 플레이어를 이름으로 찾기.
TEST_F(PlayerListTest, SearchNonExistPlayerByName)
{
	ASSERT_FALSE(test_players.searchPlayer("Tester0"));
}

// 존재하지 않는 플레이어를 uID로 찾기.
TEST_F(PlayerListTest, SearchNonExistPlayerById)
{
	ASSERT_FALSE(test_players.searchPlayer(11));
}

// 플레이어 목록 얻어오기
TEST_F(PlayerListTest, GetPlayersInfo)
{
	auto players_info = test_players.getPlayersInfo();

	ASSERT_EQ(1, std::get<0>(*(players_info->begin())));
	ASSERT_STREQ("Tester1", std::get<1>(*(players_info->begin())).c_str());
	ASSERT_STREQ("tests/user_Tester1_script.lua", std::get<2>(*(players_info->begin())).c_str());
}

// 서버에서 특정 사용자의 객체를 반환받는 동작.
TEST_F(PlayerListTest, GetExistPlayerByName)
{
	AIOnly::PlayerPtr test_player = test_players.getPlayer("Tester2");
	ASSERT_TRUE((bool)test_player);
	ASSERT_STREQ("Tester2", test_player->getName().c_str());
}

TEST_F(PlayerListTest, GetExistPlayerByID)
{
	AIOnly::PlayerPtr test_player = test_players.getPlayer(1);
	ASSERT_TRUE((bool)test_player);
	ASSERT_EQ(1, test_player->getID());
}

TEST_F(PlayerListTest, GetNonExistPlayerByName)
{
	AIOnly::PlayerPtr test_player = test_players.getPlayer("Tester0");
	ASSERT_FALSE((bool)test_player);
}

TEST_F(PlayerListTest, GetNonExistPlayerByID)
{
	AIOnly::PlayerPtr test_player = test_players.getPlayer(777);
	ASSERT_FALSE((bool)test_player);
}

// 이름을 통해 존재하는 유저 지우기.
TEST_F(PlayerListTest, RemoveExistPlayerByName)
{
	test_players.removePlayer("Tester2");
	ASSERT_FALSE(test_players.searchPlayer("Tester2"));
}

// 번호를 통해 존재하는 유저 지우기.
TEST_F(PlayerListTest, RemoveExistPlayerById)
{
	test_players.removePlayer(1);
	ASSERT_FALSE(test_players.searchPlayer(1));
}

// 이름을 통해 존재하지 않는 유저 지우기.
TEST_F(PlayerListTest, RemoveNonExistPlayerByName)
{
	test_players.removePlayer("Tester0");
	ASSERT_FALSE(test_players.searchPlayer("Tester0"));
}

// 번호를 통해 존재하지 않는 유저 지우기.
TEST_F(PlayerListTest, RemoveNonExistPlayerById)
{
	test_players.removePlayer(777);
	ASSERT_FALSE(test_players.searchPlayer(777));
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

