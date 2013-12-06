/*
 * TestGameMap.cpp
 *
 *  Created on: 2013. 12. 4.
 *      Author: hibm98
 */

#include "src/GameMap.hpp"
#include "src/MapTile.hpp"
#include "src/Avatar.hpp"
#include <gtest/gtest.h>
#include <memory>

TEST(GameMap, DefaultCreation)
{
	AIOnly::GameMap test_map;
	AIOnly::AvatarPtr tester(std::shared_ptr<AIOnly::Avatar>(new AIOnly::Avatar(1, "tester1", "")));

	ASSERT_TRUE(test_map.allocateAvatar(1, 1, tester));
	ASSERT_TRUE(test_map.getTile(1, 1).hasAvatar());
	ASSERT_STREQ(test_map.getTile(1, 1).getEnteredAvatar().getName().c_str(), "tester1");
}

TEST(GameMap, DualAvatarCreation)
{
	AIOnly::GameMap test_map;
	AIOnly::AvatarPtr tester(std::shared_ptr<AIOnly::Avatar>(new AIOnly::Avatar(1, "tester1", "")));
	AIOnly::AvatarPtr tester2(std::shared_ptr<AIOnly::Avatar>(new AIOnly::Avatar(2, "tester2", "")));

	ASSERT_TRUE(test_map.allocateAvatar(1, 1, tester));
	ASSERT_FALSE(test_map.allocateAvatar(1, 1, tester2));
	ASSERT_TRUE(test_map.getTile(1, 1).hasAvatar());
	ASSERT_STREQ(test_map.getTile(1, 1).getEnteredAvatar().getName().c_str(), "tester1");
}

TEST(GameMap, Walk_1Avatar)
{
	AIOnly::GameMap test_map;
	AIOnly::AvatarPtr tester(std::shared_ptr<AIOnly::Avatar>(new AIOnly::Avatar(1, "tester1", "")));

	test_map.allocateAvatar(1, 1, tester);
	ASSERT_TRUE(test_map.getTile(1, 1).hasAvatar());

	ASSERT_TRUE(test_map.walk(tester, AIOnly::Direction::SOUTH));
	ASSERT_FALSE(test_map.getTile(1, 1).hasAvatar());
	ASSERT_TRUE(test_map.getTile(1, 2).hasAvatar());
	ASSERT_STREQ(test_map.getTile(1, 2).getEnteredAvatar().getName().c_str(), "tester1");
}

TEST(GameMap, Walk_2Avatar)
{
	AIOnly::GameMap test_map;
	AIOnly::AvatarPtr tester(std::shared_ptr<AIOnly::Avatar>(new AIOnly::Avatar(1, "tester1", "")));
	AIOnly::AvatarPtr tester2(std::shared_ptr<AIOnly::Avatar>(new AIOnly::Avatar(2, "tester2", "")));

	test_map.allocateAvatar(1, 1, tester);
	test_map.allocateAvatar(1, 3, tester2);
	ASSERT_TRUE(test_map.getTile(1, 1).hasAvatar());

	ASSERT_TRUE(test_map.walk(tester, AIOnly::Direction::SOUTH));
	ASSERT_FALSE(test_map.walk(tester2, AIOnly::Direction::NORTH));
	ASSERT_FALSE(test_map.walk(tester, AIOnly::Direction::SOUTH));
	ASSERT_TRUE(test_map.walk(tester2, AIOnly::Direction::SOUTH));
	ASSERT_STREQ(test_map.getTile(1, 2).getEnteredAvatar().getName().c_str(), "tester1");
	ASSERT_STREQ(test_map.getTile(1, 4).getEnteredAvatar().getName().c_str(), "tester2");
}

TEST(GameMap, Attack_1Avatar)
{
	AIOnly::GameMap test_map;
	AIOnly::AvatarPtr tester(std::shared_ptr<AIOnly::Avatar>(new AIOnly::Avatar(1, "tester1", "")));

	test_map.allocateAvatar(0, 0, tester);
	ASSERT_TRUE(test_map.getTile(0, 0).hasAvatar());

	ASSERT_TRUE(test_map.attack(tester, AIOnly::Direction::SOUTH));
	ASSERT_FALSE(test_map.attack(tester, AIOnly::Direction::NORTH));
}

TEST(GameMap, Attack_2Avatar)
{
	AIOnly::GameMap test_map;
	AIOnly::AvatarPtr tester1(std::shared_ptr<AIOnly::Avatar>(new AIOnly::Avatar(1, "tester1", "")));
	AIOnly::AvatarPtr tester2(std::shared_ptr<AIOnly::Avatar>(new AIOnly::Avatar(2, "tester2", "")));

	test_map.allocateAvatar(1, 1, tester1);
	test_map.allocateAvatar(1, 3, tester2);

	ASSERT_TRUE(test_map.attack(tester1, AIOnly::Direction::SOUTH));
	ASSERT_EQ(tester2->getMaxHP(), tester2->getCurrentHP());

	test_map.walk(tester1, AIOnly::Direction::SOUTH);
	ASSERT_TRUE(test_map.attack(tester1, AIOnly::Direction::SOUTH));
	ASSERT_EQ(tester2->getMaxHP() - 1, tester2->getCurrentHP());
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
