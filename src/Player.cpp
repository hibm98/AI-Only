/*
 * Player.cpp
 *
 *  Created on: 2013. 11. 3.
 *      Author: hibm98
 */

#include "Player.hpp"

namespace AIOnly
{

Player::Player(std::string playerName, std::string codePath, int playerID, int countWin, int countLose)
	: name(playerName), codePath(codePath), isGaming(false), winCount(countWin), loseCount(countLose), id(playerID)
{
}

Player::~Player()
{
}

void Player::countWin()
{
	mutexCountWin.lock();
	winCount += 1;
	mutexCountWin.unlock();
}

void Player::countLose()
{
	mutexCountLose.lock();
	loseCount += 1;
	mutexCountLose.unlock();
}

const std::string& Player::getName() const
{
	return name;
}

const std::string& Player::getCodePath() const
{
	return codePath;
}

int Player::getWinCount() const
{
	return winCount;
}

int Player::getLoseCount() const
{
	return loseCount;
}

int Player::getID() const
{
	return id;
}

bool Player::getIsGaming() const
{
	return isGaming;
}

void Player::switchIsGaming()
{
	if (isGaming == false)
		isGaming = true;
	else
		isGaming = false;
}

std::string Player::getCode() const
{
	std::ifstream fin(this->codePath);

	// open check
	if (!fin.is_open() || !fin.good())
		return std::string();

	// read code
	return std::string(std::istreambuf_iterator<char>(fin),
				 	   std::istreambuf_iterator<char>());
}

} /* namespace AIOnly */
