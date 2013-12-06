/*
 * TestRoomList.cpp
 *
 *  Created on: 2013. 11. 13.
 *      Author: hibm98
 */

#include "../src/PlayerList.hpp"
#include "../src/RoomStatus.hpp"
#include "../src/RoomManager.hpp"
#include <gtest/gtest.h>

// Test create room

TEST(CreatePlayerTest, CreatePlayerTest)
{
	using namespace AIOnly;

	RoomManager test_room;
	int result;

	//try create player
	result = test_room.createPlayer("Tester1", "tests/user_Tester1_script.lua", 1);
	ASSERT_TRUE(result);
	result = test_room.createPlayer("Tester2", "tests/user_Tester2_script.lua", 2);
	ASSERT_TRUE(result);

	//try create already created player
	result = test_room.createPlayer("Tester1", "tests/user_Tester1_script.lua", 1);
	ASSERT_FALSE(result);
	result = test_room.createPlayer("Tester1", "tests/user_Tester2_script.lua", 2);
	ASSERT_FALSE(result);
	result = test_room.createPlayer("Tester1", "tests/user_Tester2_script.lua", 3);
	ASSERT_FALSE(result);
}


// Test manage room & start game

class PlayerManageTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		//// Add some player ////
		test_room.createPlayer("Tester1", "tests/user_Tester1_script.lua", 1);
		test_room.createPlayer("Tester2", "tests/user_Tester2_script.lua", 2);
		test_room.createPlayer("Tester3", "tests/user_Tester1_script.lua", 3);
		test_room.createPlayer("Tester4", "tests/user_Tester2_script.lua", 4);
		test_room.createPlayer("Tester5", "tests/user_Tester1_script.lua", 5);
		test_room.createPlayer("Tester6", "tests/user_Tester2_script.lua", 6);

		//// Add some room ////
		// Room 1
		premaked_RoomID = test_room.joinRoom(2);

		// Room 2
		int second_room = test_room.joinRoom(4);
		test_room.joinRoom(5, second_room);

		// Room 3
		test_room.joinRoom(6, second_room);
	}

	int premaked_RoomID;
	AIOnly::RoomManager test_room;
};

TEST_F(PlayerManageTest, ExceedRoomMax)
{
	test_room.joinRoom(3, test_room.searchPlayer("Tester4"));
	ASSERT_EQ(0, test_room.searchPlayer("Tester3"));
}

TEST_F(PlayerManageTest, CreateRoom)
{
	ASSERT_EQ(1, premaked_RoomID);
}

TEST_F(PlayerManageTest, GetAllPlayerInfo)
{
	auto info = test_room.getAllPlayerInfo();
	auto it = info->begin();

	ASSERT_EQ(std::get<0>(*it), 1);
	ASSERT_STREQ(std::get<1>(*it).c_str(), "Tester1");
	ASSERT_STREQ(std::get<2>(*it).c_str(), "tests/user_Tester1_script.lua");

	it++;
	ASSERT_EQ(std::get<0>(*it), 2);
	ASSERT_STREQ(std::get<1>(*it).c_str(), "Tester2");
	ASSERT_STREQ(std::get<2>(*it).c_str(), "tests/user_Tester2_script.lua");
}

TEST_F(PlayerManageTest, SearchNotEnteredPlayerByIdTest)
{
	ASSERT_EQ(0, test_room.searchPlayer(1));
}

TEST_F(PlayerManageTest, SearchNotEnteredPlayerByNameTest)
{
	ASSERT_EQ(0, test_room.searchPlayer("Tester0"));
}

TEST_F(PlayerManageTest, SearchEnteredPlayerByIdTest)
{
	ASSERT_EQ(premaked_RoomID, test_room.searchPlayer(2));
}

TEST_F(PlayerManageTest, SearchEnteredPlayerByNameTest)
{
	ASSERT_EQ(premaked_RoomID, test_room.searchPlayer("Tester2"));
}

TEST_F(PlayerManageTest, SearchNonExistPlayerByIdTest)
{
	ASSERT_EQ(0, test_room.searchPlayer(777));
}

TEST_F(PlayerManageTest, SearchNonExistPlayerByNameTest)
{
	ASSERT_EQ(0, test_room.searchPlayer("Tester0"));
}

TEST_F(PlayerManageTest, SearchExistRoom)
{
	ASSERT_EQ(AIOnly::RoomStatus::OPEN, test_room.getRoomStatus(premaked_RoomID).getStatus());
}

TEST_F(PlayerManageTest, SearchNonExistRoom)
{
	ASSERT_EQ(AIOnly::RoomStatus::NON_EXIST, test_room.getRoomStatus(55).getStatus());
}

TEST_F(PlayerManageTest, GetExistRoomStatus)
{
	auto room = test_room.getRoomStatus(1);
	ASSERT_EQ(AIOnly::RoomStatus::OPEN, room.getStatus());
}

TEST_F(PlayerManageTest, GetNonExistRoomStatus)
{
	auto room = test_room.getRoomStatus(55);
	ASSERT_EQ(AIOnly::RoomStatus::NON_EXIST, room.getStatus());
}

TEST_F(PlayerManageTest, CreateRoomByID)
{
	ASSERT_LT(0, test_room.joinRoom(1));
	ASSERT_LT(0, test_room.searchPlayer(1));
}

TEST_F(PlayerManageTest, CreateRoomByName)
{
	ASSERT_LT(0, test_room.joinRoom("Tester1"));
	ASSERT_LT(0, test_room.searchPlayer("Tester1"));
}

TEST_F(PlayerManageTest, JoinRoomByID)
{
	ASSERT_EQ(1, test_room.joinRoom(3, 1));
	ASSERT_EQ(1, test_room.searchPlayer(3));
	ASSERT_EQ(1, test_room.searchPlayer(2));
}

TEST_F(PlayerManageTest, JoinRoomByName)
{
	ASSERT_EQ(1, test_room.joinRoom("Tester3", 1));
	ASSERT_EQ(1, test_room.searchPlayer("Tester3"));
	ASSERT_EQ(1, test_room.searchPlayer("Tester2"));
}

TEST_F(PlayerManageTest, LeaveRoomByID)
{
	test_room.leaveRoom(5);
	ASSERT_EQ(0, test_room.searchPlayer(5));
	ASSERT_EQ(2, test_room.joinRoom(5, 2));
	ASSERT_EQ(test_room.searchPlayer(4), test_room.searchPlayer(5));
}

TEST_F(PlayerManageTest, LeaveRoomByName)
{
	test_room.leaveRoom("Tester5");
	ASSERT_EQ(0, test_room.searchPlayer("Tester5"));
	ASSERT_EQ(2, test_room.joinRoom("Tester5", 2));
	ASSERT_EQ(test_room.searchPlayer("Tester4"), test_room.searchPlayer("Tester5"));
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
