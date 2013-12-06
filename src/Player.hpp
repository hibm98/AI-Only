/*
 * Player.hpp
 *
 *  Created on: 2013. 11. 3.
 *      Author: hibm98
 */

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <fstream>
#include <string>
#include <memory>
#include <mutex>

namespace AIOnly
{

/*
 *
 */
class Player
{
	int id;
	std::string name;
	std::string codePath;

	bool isGaming;

	int winCount;
	int loseCount;

	std::mutex mutexCountWin;
	std::mutex mutexCountLose;

public:
	Player(std::string playerName, std::string codePath, int playerID, int countWin = 0, int countLose = 0);
	virtual ~Player();

	int getID() const;
	const std::string& getName() const;
	const std::string& getCodePath() const;

	bool getIsGaming() const;
	void switchIsGaming();

	int getWinCount() const;
	int getLoseCount() const;
	void countWin();
	void countLose();

	std::string getCode() const;
};

typedef std::shared_ptr<Player> PlayerPtr;

} /* namespace AIOnly */

#endif /* PLAYER_HPP_ */
