/*
 * RoomStatus.hpp
 *
 *  Created on: 2013. 11. 17.
 *      Author: hibm98
 */

#ifndef ROOMSTATUS_HPP_
#define ROOMSTATUS_HPP_

namespace AIOnly
{

class RoomStatus {
	int _roomID;	// 방 번호
	int _status;	// 방의 상태
	int _current;	// 현재 인원
	int _max;		// 최대 인원

public:
	enum Status {
		NON_EXIST	= 0,
		OPEN		= 1,
		STARTED		= 2
	};

	RoomStatus(int roomID);
	RoomStatus(int roomID, int status, int current, int max);
	int getRoomID() const;
	int getStatus() const;
	int getCurrent() const;
	int getMax() const;
};

}
#endif /* ROOMSTATUS_HPP_ */
