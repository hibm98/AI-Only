/*
 * RoomStatus.cpp
 *
 *  Created on: 2013. 11. 17.
 *      Author: hibm98
 */

#include "RoomManager.hpp"

namespace AIOnly
{

RoomStatus::RoomStatus(int roomID)
	: _roomID(roomID), _status(NON_EXIST), _current(0), _max(0)
{
}

RoomStatus::RoomStatus(int roomID, int status, int current, int max)
	: _roomID(roomID), _status(status), _current(current), _max(max)
{
}

int RoomStatus::getRoomID() const
{
	return _roomID;
}

int RoomStatus::getStatus() const
{
	return _status;
}

int RoomStatus::getCurrent() const
{
	return _current;
}

int RoomStatus::getMax() const
{
	return _max;
}

}
