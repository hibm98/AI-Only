/*
 * PlayerList.cpp
 *
 *  Created on: 2013. 11. 14.
 *      Author: hibm98
 */

#include "PlayerList.hpp"

namespace AIOnly
{

PlayerList::PlayerList()
{
	// TODO Auto-generated constructor stub

}

PlayerList::~PlayerList()
{
	// TODO Auto-generated destructor stub
}

bool PlayerList::searchPlayer_inLock(int playerID) const
{
	return (playerID > 0 && players.find(playerID) != players.end() ? true : false);
}

bool PlayerList::searchPlayer(int playerID) const
{
	if (playerID > 0)
	{ 	// Critical section START
		std::unique_lock<std::mutex> listLock(listLockMutex, std::adopt_lock);
		return this->searchPlayer_inLock(playerID);
	}	// Critical section END
	else
		return false;
}

void PlayerList::removePlayer(int playerID)
{
	if (playerID > 0)
	{	// CRITICAL SECTION START
		std::unique_lock<std::mutex> listLock(listLockMutex, std::adopt_lock);
		if (this->searchPlayer_inLock(playerID))
			players.erase(playerID);
	}	// CRITICAL SECTION END

}

int PlayerList::insertPlayer(PlayerPtr player)
{	// CRITICAL SECTION START
	std::unique_lock<std::mutex> listLock(listLockMutex, std::adopt_lock);

	if (!this->searchPlayer_inLock(player->getID()) && !this->getPlayer_inLock(player->getName()))
	{
		players[player->getID()] = player;
		return 1;
	}
	else
	{
		return 0;
	}
}	// CRITICAL SECTION END

PlayerPtr PlayerList::getPlayer_inLock(int playerID) const
{
	PlayerPtr result;				// 키가 없을 경우에 대비해서 비어있는 반환값을 생성한다.

	auto it = players.find(playerID);
	if (it != players.end())		// 만약 키가 존재한다면 그 값을 반환하도록 설정한다.
		result = it->second;

	return result;
}

PlayerPtr PlayerList::getPlayer_inLock(const std::string& playerName) const
{
	PlayerPtr result;

	if(!playerName.empty())
	{
		for(auto it = players.begin(); it != players.end(); it++)
		{
			if(playerName == it->second->getName())
				result = it->second;
		}
	}

	 return result;
}

PlayerPtr PlayerList::getPlayer(int playerID) const
{	// CRITICAL SECTION START
	std::unique_lock<std::mutex> listLock(listLockMutex, std::adopt_lock);
	return this->getPlayer_inLock(playerID);
}	// CRITICAL SECTION END

PlayerPtr PlayerList::getPlayer(const std::string& playerName) const
{	// CRITICAL SECTION START
	std::unique_lock<std::mutex> listLock(listLockMutex, std::adopt_lock);
	return this->getPlayer_inLock(playerName);
}	// CRITICAL SECTION END

const PlayersInfoPtr PlayerList::getPlayersInfo() const
{
	PlayersInfoPtr playersInfo(new PlayersInfo);

	{	// CRITICAL SECTION START
		std::unique_lock<std::mutex> listLock(listLockMutex, std::adopt_lock);
		for(auto it = players.begin(); it != players.end(); it++)
		{
			auto& player = it->second;
			playersInfo->push_back(std::make_tuple(player->getID(), player->getName(), player->getCodePath()));
		}
	}	// CRITICAL SECITON END

	// check zero-length vector before sorting vector
	if (!playersInfo->empty())
	{
		// sorting vector
		std::sort(playersInfo->begin(), playersInfo->end(),
					[](PlayerInfo a, PlayerInfo b)
					{ return std::get<0>(a) < std::get<0>(b); }
				 );
	}

	return playersInfo;
}

} /* namespace AIOnly */
