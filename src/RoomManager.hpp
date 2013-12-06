/*
 * RoomList.h
 *
 *  Created on: 2013. 11. 13.
 *      Author: hibm98
 */

#ifndef ROOMMANAGER_HPP_
#define ROOMMANAGER_HPP_

#include <memory>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <mutex>
#include <thread>

#include "Player.hpp"
#include "PlayerList.hpp"
#include "RoomStatus.hpp"
#include "Game.hpp"

namespace AIOnly
{

// 방 목록 자료형 정의
typedef std::tuple<int /*roomID*/, RoomStatus::Status, int /*current*/, int /*max*/> RoomInfo;
typedef std::vector<RoomInfo> RoomsInfo;
typedef std::shared_ptr<RoomsInfo> RoomsInfoPtr;

// 방 목록 튜플의 데이터 순서
enum RoomInfo_TupleType { ROOMID = 0, STATUS, CURRENT, MAX };

/*
 *
 */
class RoomManager
{
	// Room Control
	std::unordered_map<int /*roomID*/,
					   std::tuple<PlayerListPtr,
					   	   	   	  /*GamePtr,*/
					   	   	   	  RoomStatus::Status,
					   	   	   	  int /*current*/,
					   	   	   	  int /*max*/>
					  > rooms;
	enum RoomPrivateInfo_TupleType { PLAYERLIST = 0, /*GAME,*/ STATUS, CURRENT, MAX };

	int roomNumber = 0;
	mutable std::mutex room_control_mutex;

	// Player Control
	PlayerList all_players;
	std::unordered_map<int /*playerID*/,
					   int /*roomID*/
					  > current_room_in_player;
	mutable std::mutex player_control_mutex;

	RoomStatus getRoomStatus_inLock(int roomID) const;

public:
	static const int MAX_PLAYER_IN_ROOM = 2;

	RoomManager();
	virtual ~RoomManager();

	// 방을 들어오고 나갈 때 호출될 메소드.
	int joinRoom(int playerID, int roomID = 0);
	int joinRoom(const std::string& playerName, int roomID = 0);

	int leaveRoom(int playerID);
	int leaveRoom(const std::string& playerName);

	// 특정 플레이어가 어느 방에 있는지 찾는 메소드.
	int searchPlayer(int playerID) const;
	int searchPlayer(const std::string& playerName) const;

	// 특정 방의 상태를 확인하는 메소드.
	RoomStatus getRoomStatus(int roomID) const;

	// 게임 시작용 메소드. (stub)
	int startGame(int roomID);

	// 방 목록 관리자가 담당할 플레이어의 추가/제거용 메소드.
	int createPlayer(const std::string& playerName, const std::string& codePath, int playerID, int countWin = 0, int countLose = 0);
	int removePlayer(int playerID);
	int removePlayer(const std::string& playerName);

	// 플레이어 및 방 목록을 반환하는 메소드.
	const PlayersInfoPtr getAllPlayerInfo() const;
	const RoomsInfoPtr getAllRoomInfo() const;
};

} /* namespace AIOnly */

#endif /* ROOMMANAGER_HPP_*/
