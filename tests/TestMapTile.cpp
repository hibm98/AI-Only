/*
 * TestMapTile.cpp
 *
 *  Created on: 2013. 12. 4.
 *      Author: hibm98
 */

#include "src/MapTile.hpp"
#include "src/Avatar.hpp"
#include <gtest/gtest.h>
#include <memory>

TEST(MapTileCreation, DefaultCreation)
{
	AIOnly::MapTile test_tile;

	int result = (AIOnly::TileType::DIRT == test_tile.getTileType());
	ASSERT_EQ(1, result);
}

TEST(MapTileTest, AllocateAvatar)
{
	AIOnly::AvatarPtr tester(std::shared_ptr<AIOnly::Avatar>(new AIOnly::Avatar(1, "tester1", "")));
	AIOnly::MapTile test_tile;

	ASSERT_TRUE(AIOnly::allocateAvatar(test_tile, tester));
	ASSERT_TRUE(test_tile.hasAvatar());
	ASSERT_STREQ(test_tile.getEnteredAvatar().getName().c_str(), "tester1");
}

TEST(MapTileTest, MoveAvatar)
{
	AIOnly::AvatarPtr tester(std::shared_ptr<AIOnly::Avatar>(new AIOnly::Avatar(1, "tester1", "")));
	AIOnly::MapTile test_tile1;
	AIOnly::MapTile test_tile2;

	AIOnly::allocateAvatar(test_tile1, tester);
	ASSERT_TRUE(AIOnly::moveAvatar(test_tile1, test_tile2));
	ASSERT_FALSE(test_tile1.hasAvatar());
	ASSERT_TRUE(test_tile2.hasAvatar());
	ASSERT_STREQ(test_tile2.getEnteredAvatar().getName().c_str(), "tester1");
}

TEST(MapTileTest, AttackedAvatar)
{
	AIOnly::MapTile test_tile;
	AIOnly::AvatarPtr pAvt(std::shared_ptr<AIOnly::Avatar>(new AIOnly::Avatar(1, "tester1", "")));

	AIOnly::allocateAvatar(test_tile, pAvt);
	test_tile.attacked(5);
	ASSERT_EQ(pAvt->getMaxHP() - 5, test_tile.getEnteredAvatar().getCurrentHP());
	ASSERT_EQ(pAvt->getMaxHP() - 5, pAvt->getCurrentHP());
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
