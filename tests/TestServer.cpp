#include <gtest/gtest.h>
#include "../src/server.hpp"

class ConnectionTest : public testing::Test {
protected:
	virtual void SetUp() {
		test_server = new AIOnly::Server(63000);
	}
	
	virtual void TearDown() {
		test_server->close();
		delete test_server;
	}

	AIOnly::Server *test_server;
};


TEST_F(ConnectionTest, InitializeTest) {
	ASSERT_EQ(FALSE, test_server->getStatus());
	test_server->open();
	ASSERT_EQ(TRUE, test_server->getStatus());
	test_server->close();
	ASSERT_EQ(FALSE, test_server->getStatus());
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
