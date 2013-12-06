/*
 * PlayerList.hpp
 *
 *  Created on: 2013. 11. 14.
 *      Author: hibm98
 */

#ifndef PLAYERLIST_HPP_
#define PLAYERLIST_HPP_

#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <memory>
#include <mutex>
#include <algorithm>

#include "Player.hpp"

namespace AIOnly
{

typedef std::tuple<int /*PlayerID*/, std::string /*PlayerName*/, std::string /*PlayerScriptSrc*/> PlayerInfo;
typedef std::vector<PlayerInfo> PlayersInfo;
typedef std::shared_ptr<PlayersInfo> PlayersInfoPtr;

enum PlayerInfo_TupleType
{
	PID = 0,
	PNAME,
	PSOURCE
};

/*
 *
 */
class PlayerList
{
	std::unordered_map<int, PlayerPtr> players;
	mutable std::mutex listLockMutex;

	bool searchPlayer_inLock(int playerID) const;
	inline bool searchPlayer_inLock(const PlayerPtr player) const
		{ return (player ? this->searchPlayer_inLock(player->getID()) : false); }
	inline bool searchPlayer_inLock(const std::string& playerName) const
		{ return searchPlayer_inLock(getPlayer_inLock(playerName)); }
	PlayerPtr getPlayer_inLock(int playerID) const;
	PlayerPtr getPlayer_inLock(const std::string& playerName) const;

public:
	PlayerList();
	virtual ~PlayerList();

	int insertPlayer(const PlayerPtr player);

	bool searchPlayer(int playerID) const;
	inline bool searchPlayer(const PlayerPtr player) const
		{ return (player ? this->searchPlayer(player->getID()) : false); }
	inline bool searchPlayer(const std::string& playerName) const
		{ return searchPlayer(getPlayer(playerName)); }

	const PlayersInfoPtr getPlayersInfo() const;

	PlayerPtr getPlayer(int playerID) const;
	PlayerPtr getPlayer(const std::string& playerName) const;

	void removePlayer(int playerID);
	inline void removePlayer(const PlayerPtr player)
		{ removePlayer(player ? player->getID() : 0); }
	inline void removePlayer(const std::string& playerName)
		{ removePlayer(getPlayer(playerName)); }
};

typedef std::shared_ptr<PlayerList> PlayerListPtr;

} /* namespace AIOnly */

#endif /* PLAYERLIST_HPP_ */
