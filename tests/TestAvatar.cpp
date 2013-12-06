/*
 * TestAvatar.cpp
 *
 *  Created on: 2013. 11. 3.
 *      Author: hibm98
 */

#include <gtest/gtest.h>
#include "src/Avatar.hpp"
#include <functional>

TEST(AvatarCreateTest, NamedAvatarCreate)
{
	AIOnly::Avatar test_avatar(1, "Tester1", "tests/Tester1.lua");

	ASSERT_STREQ("Tester1", test_avatar.getName().c_str());
	ASSERT_EQ(5, test_avatar.getCurrentSP());
	ASSERT_EQ(5, test_avatar.getMaxSP());
	ASSERT_EQ(100, test_avatar.getCurrentHP());
	ASSERT_EQ(100, test_avatar.getMaxHP());
}

/*TEST(AvatarCreateTest, NamedAvatarWithWrongSourceCreate)
{
	AIOnly::Avatar test_avatar(1, "Tester1", "tests/wrong-source.lua");

	ASSERT_STREQ("Tester1", test_avatar.getName().c_str());
	ASSERT_EQ(5, test_avatar.getCurrentSP());
	ASSERT_EQ(5, test_avatar.getMaxSP());
	ASSERT_EQ(100, test_avatar.getCurrentHP());
	ASSERT_EQ(100, test_avatar.getMaxHP());
	ASSERT_FALSE(test_avatar.hasAI());
}*/

TEST(AvatarCreateTest, NamedWithMaxSPAndMaxHPAvatarCreate)
{
	AIOnly::Avatar test_avatar(1, "Tester1", "tests/Tester1.lua", 10, 50);

	ASSERT_STREQ("Tester1", test_avatar.getName().c_str());
	ASSERT_EQ(10, test_avatar.getCurrentSP());
	ASSERT_EQ(10, test_avatar.getMaxSP());
	ASSERT_EQ(50, test_avatar.getCurrentHP());
	ASSERT_EQ(50, test_avatar.getMaxHP());
}

TEST(AvatarTest, SuccessedAction)
{

	AIOnly::Avatar test_avatar = AIOnly::Avatar(1, "Tester1", "tests/Tester1.lua");
	std::function<bool()> dummy_func = [=]() { return true; };

	// check full SP
	ASSERT_EQ(test_avatar.getMaxSP(), test_avatar.getCurrentSP());

	// act
	test_avatar.action(5, dummy_func);

	// check SP
	ASSERT_EQ(test_avatar.getMaxSP()-5, test_avatar.getCurrentSP());
}

TEST(AvatarTest, FailedActionBecauseNotEnoughSP)
{
	AIOnly::Avatar test_avatar = AIOnly::Avatar(1, "Tester1", "tests/Tester1.lua");
	std::function<bool()> dummy_func = [=]() { return true; };

	// check full SP
	ASSERT_EQ(test_avatar.getMaxSP(), test_avatar.getCurrentSP());

	// use small SP
	test_avatar.action(2, dummy_func);

	// use big SP (it will failed)
	test_avatar.action(15, dummy_func);

	// check SP
	ASSERT_EQ(test_avatar.getMaxSP()-2, test_avatar.getCurrentSP());
}

TEST(AvatarTest, RestTest)
{
	AIOnly::Avatar test_avatar = AIOnly::Avatar(1, "Tester1", "tests/Tester1.lua");
	std::function<bool()> dummy_func = [=]() { return true; };

	// use SP and rest
	test_avatar.action(4, dummy_func);
	test_avatar.rest();

	// check SP
	ASSERT_EQ(test_avatar.getMaxSP(), test_avatar.getCurrentSP());
}

TEST(AvatarTest, AttackedTest)
{
	AIOnly::Avatar test_avatar = AIOnly::Avatar(1, "Tester1", "tests/Tester1.lua");
	std::function<bool()> dummy_func = [=]() { return true; };

	// check HP
	ASSERT_EQ(test_avatar.getMaxHP(), test_avatar.getCurrentHP());

	// attack
	test_avatar.attacked(33);

	// re-check HP
	ASSERT_EQ(test_avatar.getMaxHP()-33, test_avatar.getCurrentHP());
	ASSERT_FALSE(test_avatar.isDead());
}

TEST(AvatarTest, DieTest)
{
	AIOnly::Avatar test_avatar = AIOnly::Avatar(1, "Tester1", "tests/Tester1.lua");
	std::function<bool()> dummy_func = [=]() { return true; };

	// check HP
	ASSERT_EQ(test_avatar.getMaxHP(), test_avatar.getCurrentHP());

	// attack (it will die)
	test_avatar.attacked(999);

	// dead check
	ASSERT_TRUE(test_avatar.isDead());
	ASSERT_EQ(0, test_avatar.getCurrentHP());
}

TEST(AvatarTest, HealedTest)
{
	AIOnly::Avatar test_avatar = AIOnly::Avatar(1, "Tester1", "tests/Tester1.lua");
	std::function<bool()> dummy_func = [=]() { return true; };

	// check HP
	ASSERT_EQ(test_avatar.getMaxHP(), test_avatar.getCurrentHP());

	// attack
	test_avatar.attacked(99);

	// re-check HP
	ASSERT_EQ(test_avatar.getMaxHP()-99, test_avatar.getCurrentHP());
	ASSERT_FALSE(test_avatar.isDead());

	// heal
	test_avatar.healed(55);

	// re-check HP
	ASSERT_EQ(test_avatar.getMaxHP()-99+55, test_avatar.getCurrentHP());

	test_avatar.healed(999);
	ASSERT_EQ(test_avatar.getMaxHP(), test_avatar.getCurrentHP());
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
