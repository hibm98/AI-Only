/*
 * Server.h
 *
 *  Created on: 2013. 11. 3.
 *      Author: hibm98
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <map>
#include <memory>
#include <mutex>
#include <list>

#include "PlayerList.hpp"
#include "RoomManager.hpp"

namespace AIOnly
{

/*
 *
 */
class Server
{
	RoomManager rooms;

public:
	Server();
	~Server();
};

} /* namespace AIOnly */

#endif /* SERVER_H_ */
