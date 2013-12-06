/*
 * TestRunScriptWithHook.cpp
 *
 *  Created on: 2013. 12. 3.
 *      Author: hibm98
 */

#include "../src/luacppinterface/luacppinterface.h"
#include <string>
#include <gtest/gtest.h>

TEST(LuaCppInterfaceTest, RunScriptWithScript)
{
	Lua l;

	ASSERT_STREQ("Error: Time exceed.\n", l.RunScriptWithHook("while true do a=1 end", 100).c_str());
	ASSERT_STREQ("Error: Time exceed.\n", l.RunScriptWithHook("while true do a=1 end", 100).c_str());
	ASSERT_STREQ("No errors", l.RunScriptWithHook("a=1", 100).c_str());
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
