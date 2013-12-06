# VARIABLES
Require_library = lib/libaionly.a \
				  lib/libluacppinterface.a
Test_library = gtest

# LIBRARYS
lib_LIBRARIES = lib/libluacppinterface.a \
				lib/libaionly.a
				
lib_libluacppinterface_a_SOURCES = src/luacppinterface/luacoroutine.cpp \
								   src/luacppinterface/luacppinterface.cpp \
								   src/luacppinterface/luafunction.cpp \
								   src/luacppinterface/luareference.cpp \
								   src/luacppinterface/luatable.cpp
								   
lib_libaionly_a_SOURCES = src/Avatar.cpp \
						  src/Game.cpp \
						  src/GameMap.cpp \
						  src/GameResult.cpp \
						  src/MapTile.cpp \
						  src/Player.cpp \
						  src/PlayerList.cpp \
						  src/RoomStatus.cpp \
						  src/RoomManager.cpp \
						  src/CmdParser.cpp \
						  src/ServerCmdParser.cpp

# BINARY
bin_PROGRAMS = bin/aionly
bin_aionly_SOURCES = src/CLI_main.cpp
bin_aionly_LDADD = $(Require_library)

# TESTS
TESTS = $(check_PROGRAMS)
check_PROGRAMS = tests/bin/TestAvatar \
				 tests/bin/TestGame \
				 tests/bin/TestPlayer \
				 tests/bin/TestPlayerList \
				 tests/bin/TestRoomManager \
				 tests/bin/TestServerCmdParser \
				 tests/bin/TestRunScriptWithHook \
				 tests/bin/TestMapTile \
				 tests/bin/TestGameMap

tests_bin_TestAvatar_SOURCES = tests/TestAvatar.cpp
tests_bin_TestAvatar_LDADD = $(Require_library) -l${Test_library}

tests_bin_TestGame_SOURCES = tests/TestGame.cpp
tests_bin_TestGame_LDADD = $(Require_library) -l${Test_library}

tests_bin_TestPlayer_SOURCES = tests/TestPlayer.cpp
tests_bin_TestPlayer_LDADD = $(Require_library) -l${Test_library}

tests_bin_TestPlayerList_SOURCES = tests/TestPlayerList.cpp
tests_bin_TestPlayerList_LDADD = $(Require_library) -l${Test_library}

tests_bin_TestRoomManager_SOURCES = tests/TestRoomManager.cpp
tests_bin_TestRoomManager_LDADD = $(Require_library) -l${Test_library}

tests_bin_TestServerCmdParser_SOURCES = tests/TestServerCmdParser.cpp
tests_bin_TestServerCmdParser_LDADD = $(Require_library) -l${Test_library}

tests_bin_TestRunScriptWithHook_SOURCES = tests/TestRunScriptWithHook.cpp
tests_bin_TestRunScriptWithHook_LDADD = $(Require_library) -l$(Test_library)

tests_bin_TestMapTile_SOURCES = tests/TestMapTile.cpp
tests_bin_TestMapTile_LDADD = $(Require_library) -l$(Test_library)

tests_bin_TestGameMap_SOURCES = tests/TestGameMap.cpp
tests_bin_TestGameMap_LDADD = $(Require_library) -l$(Test_library)

# tests_bin_TestServer_SOURCES = tests/TestServer.cpp
# tests_bin_TestServer_LDADD = $(Require_library) -l${Test_library}