/*
 * RoomManager.cpp
 *
 *  Created on: 2013. 11. 13.
 *      Author: hibm98
 */

#include "RoomManager.hpp"

namespace AIOnly
{

RoomManager::RoomManager()
{
	// TODO Auto-generated constructor stub

}

RoomManager::~RoomManager()
{
	// TODO Auto-generated destructor stub
}

int RoomManager::searchPlayer(int playerID) const
{ // Critical section START
	std::unique_lock<std::mutex> lock(this->player_control_mutex);
	auto it = current_room_in_player.find(playerID);

	if (it != current_room_in_player.end())
		return it->second;
	else
		return 0;
} // Critical section END

int RoomManager::searchPlayer(const std::string& playerName) const
{
	PlayerPtr player = all_players.getPlayer(playerName);

	if (player)
		return this->searchPlayer(player->getID());
	else
		return 0;
}

RoomStatus RoomManager::getRoomStatus_inLock(int roomID) const
{
	if (roomID > 0)
	{
		auto it = rooms.find(roomID);
		if(it != rooms.end())
			return RoomStatus(roomID,
							  std::get<RoomPrivateInfo_TupleType::STATUS> (it->second),
							  std::get<RoomPrivateInfo_TupleType::CURRENT>(it->second),
							  std::get<RoomPrivateInfo_TupleType::MAX>    (it->second));
		else
			return RoomStatus(roomID);
	}
	else
	{
		return RoomStatus(roomID);
	}
}

RoomStatus RoomManager::getRoomStatus(int roomID) const
{
	// CRITICAL SECTION START
	std::unique_lock<std::mutex> roomLock(room_control_mutex);
	return this->getRoomStatus_inLock(roomID);
	// CRITICAL SECTION END
}

int RoomManager::joinRoom(const std::string& playerName, int roomID)
{
	PlayerPtr player = all_players.getPlayer(playerName);

	if (!player)
		return 0;
	else
		return this->joinRoom(player->getID(), roomID);
}

int RoomManager::joinRoom(int playerID, int roomID)
{
	if (roomID < 0 || !all_players.searchPlayer(playerID))
		return 0;

	// 사용자가 이미 다른 방에 들어가 있는지 확인.
	if (this->searchPlayer(playerID))
		return -1;

	if (roomID > 0)
	{
		// 방이 존재하고 아직 시작하지 않았는지 체크.
		RoomStatus target = this->getRoomStatus(roomID);
		if (target.getStatus() != RoomStatus::OPEN)
			return -2;
	}

	// 있으면 방 조작 뮤텍스를 잠금.
	int _roomID;

	{	// CRITICAL SECTION START
		std::unique_lock<std::mutex> roomLock(room_control_mutex, std::defer_lock);
		std::unique_lock<std::mutex> playerLock(player_control_mutex, std::defer_lock);
		std::lock(roomLock, playerLock);	// Anti-Deadlock

		if (roomID > 0)
		{
			// 다시 방이 있는지 검사 (방 조작을 잠그기 직전에 상태가 변경되었는지 확인)
			RoomStatus target = this->getRoomStatus_inLock(roomID);

			// 방에 여유 공간이 있는지 검사
			if (target.getCurrent() >= target.getMax())
				return -2;

			// 방이 열려있는지 검사
			if (target.getStatus() != RoomStatus::OPEN)
				return -2;

			// 모든 검사를 통과하였으면 방 참가 프로세스를 계속 진행함.
			_roomID = roomID;
		}
		else
		{
			// 방을 만들어야 하는 경우 방을 만든다.
			rooms[++roomNumber] = std::make_tuple(std::make_shared<PlayerList>(),
												  /*std::make_shared<Game>(),*/
												  RoomStatus::OPEN,
												  0,
												  MAX_PLAYER_IN_ROOM);
			_roomID = roomNumber;
		}

		// 최종적으로 방에 사용자를 집어 넣음.
		std::get<RoomPrivateInfo_TupleType::PLAYERLIST>(rooms[_roomID])->insertPlayer(all_players.getPlayer(playerID));
		std::get<RoomPrivateInfo_TupleType::CURRENT>   (rooms[_roomID]) += 1;
		current_room_in_player[playerID] = _roomID;
	}	// CRITICAL SECTION END

	return _roomID;
}

int RoomManager::leaveRoom(const std::string& playerName)
{
	// 해당 이름을 가진 플레이어를 찾아 ID를 이용해 나가게 한다.
	PlayerPtr player = all_players.getPlayer(playerName);
	if (!player)
		return -1;

	return this->leaveRoom(player->getID());
}

int RoomManager::leaveRoom(int playerID)
{
	// 방에 입장하지 않은 상태이면 바로 반환한다.
	if (!all_players.searchPlayer(playerID))
		return -1;

	int roomID = this->searchPlayer(playerID);
	if (roomID == 0)
		return -1;

	// 락을 걸고 방에서 빼내서 대기실(0번)로 이동시킨다.
	{	// CRITICAL SECTION START
		std::unique_lock<std::mutex> roomLock(room_control_mutex, std::defer_lock);
		std::unique_lock<std::mutex> playerLock(player_control_mutex, std::defer_lock);
		std::lock(roomLock, playerLock);	// Anti-Deadlock

		std::get<RoomPrivateInfo_TupleType::PLAYERLIST>(rooms[roomID])->removePlayer(all_players.getPlayer(playerID));
		std::get<RoomPrivateInfo_TupleType::CURRENT>   (rooms[roomID]) -= 1;
		current_room_in_player[playerID] = 0;
	}	// CRITICAL SECTION END
	return 0;
}

int RoomManager::startGame(int roomID)
{
	Game g(std::get<RoomPrivateInfo_TupleType::PLAYERLIST>(rooms[roomID]));
	g.run();
	return 0;
}

int RoomManager::createPlayer(const std::string& playerName, const std::string& codePath, int playerID, int countWin, int countLose)
{

	// 전체 플레이어 목록에 플레이어를 추가한다.
	int result = all_players.insertPlayer(std::make_shared<Player>(playerName,
																   codePath,
																   playerID,
																   countWin,
																   countLose));
	if (!result)
		return 0;

	// 추가에 성공한 경우에만 현재 방 번호를 할당한다.
	{ // Critical section START
		std::lock_guard<std::mutex> lock(player_control_mutex);
		current_room_in_player[playerID] = 0;
	} // Critical section END
	return result;
}

int RoomManager::removePlayer(int playerID)
{
	{ // Critical section START
		std::lock_guard<std::mutex> lock(player_control_mutex);
		if(current_room_in_player.count(playerID) > 0)
			current_room_in_player.erase(playerID);
	} // Critical section END

	all_players.removePlayer(playerID);
	return 0;
}

int RoomManager::removePlayer(const std::string& playerName)
{
	auto target = all_players.getPlayer(playerName);
	if(!target)
		return -1;

	return this->removePlayer(target->getID());
}

const PlayersInfoPtr RoomManager::getAllPlayerInfo() const
{
	return all_players.getPlayersInfo();
}

const RoomsInfoPtr RoomManager::getAllRoomInfo() const
{
	RoomsInfoPtr info(new RoomsInfo());

	{	// CRITICAL SECTION START
		std::unique_lock<std::mutex> roomLock(room_control_mutex, std::adopt_lock);
		for(auto it = rooms.begin(); it != rooms.end(); it++)
		{
			auto& roomInfo = it->second;
			info->push_back(std::make_tuple(it->first,
											std::get<RoomPrivateInfo_TupleType::STATUS>	(roomInfo),
											std::get<RoomPrivateInfo_TupleType::CURRENT>(roomInfo),
											std::get<RoomPrivateInfo_TupleType::MAX>	(roomInfo)));
		}
	}	// CRITICAL SECTION END

	return info;
}

} /* namespace AIOnly */
