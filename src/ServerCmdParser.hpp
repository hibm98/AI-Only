/*
 * ServerCmdParser.hpp
 *
 *  Created on: 2013. 11. 17.
 *      Author: hibm98
 */

#ifndef SERVERCMDPARSER_HPP_
#define SERVERCMDPARSER_HPP_

#include <string>
#include "CmdParser.hpp"

namespace AIOnly
{

enum ServerCmdType
{
	/* System */
	UNKNOWN,
	SAY,
	CLI_EXIT,
	CLI_HELP,

	/* Player */
	CREATE_PLAYER,
	DELETE_PLAYER,
	PLAYER_LIST,

	/* Room */
	JOIN_ROOM,
	LEAVE_ROOM,
	ROOM_LIST,

	/* Game */
	START_GAME,
	VIEW_REPORT
};

class ServerCmdParser : public CmdParser
{
protected:
	CmdType	parseCmdType(const std::string& cmd);
public:
	virtual ~ServerCmdParser();
};

} /* namespace AIOnly */

#endif /* SERVERCMDPARSER_HPP_ */
