/*
 * ServerCmdParser.cpp
 *
 *  Created on: 2013. 11. 27.
 *      Author: hibm98
 */

#include "ServerCmdParser.hpp"

namespace AIOnly
{

ServerCmdParser::~ServerCmdParser()
{
}

CmdType ServerCmdParser::parseCmdType(const std::string& cmd)
{
	CmdType cmdType;

	if (cmd == "say")
		cmdType = ServerCmdType::SAY;

	else if (cmd == "create_player")
		cmdType = ServerCmdType::CREATE_PLAYER;

	else if (cmd == "delete_player")
		cmdType = ServerCmdType::DELETE_PLAYER;

	else if (cmd == "player_list")
		cmdType = ServerCmdType::PLAYER_LIST;

	else if (cmd == "join_room")
		cmdType = ServerCmdType::JOIN_ROOM;

	else if (cmd == "leave_room")
		cmdType = ServerCmdType::LEAVE_ROOM;

	else if (cmd == "room_list")
		cmdType = ServerCmdType::ROOM_LIST;

	else if (cmd == "start_game")
		cmdType = ServerCmdType::START_GAME;

	else if (cmd == "view_report")
		cmdType = ServerCmdType::VIEW_REPORT;

	else if (cmd == "help")
		cmdType = ServerCmdType::CLI_HELP;

	else if (cmd == "exit")
		cmdType = ServerCmdType::CLI_EXIT;

	else
		cmdType = ServerCmdType::UNKNOWN;

	return cmdType;
}
} /* namespace AIOnly */
