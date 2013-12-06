/*
 * Game.hpp
 *
 *  Created on: 2013. 11. 3.
 *      Author: hibm98
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#include <memory>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <string>

#include "luacppinterface/luacppinterface.h"

#include "PlayerList.hpp"
#include "Avatar.hpp"
#include "GameMap.hpp"
#include "MapTile.hpp"
#include "GameResult.hpp"

namespace AIOnly
{

/*
 *
 */
class Game
{
	PlayerListPtr _players;
	std::vector<AvatarPtr> avatar_queue;
	GameMap map;
	GameResult result;
	std::string whyDraw;

	void prepare();
	void ingame();
	void cleanup();

public:
	Game(const PlayerListPtr players);
	virtual ~Game();

	void run();

	bool isFinished() const;
	const GameResult& getResult() const;
};

} /* namespace AIOnly */

#endif /* GAME_HPP_ */
