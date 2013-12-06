/*
 * Avatar.cpp
 *
 *  Created on: 2013. 11. 3.
 *      Author: hibm98
 */

#include "Avatar.hpp"

namespace AIOnly
{

Avatar::Avatar(int avatarID,
			   const std::string& avatarName,
			   const std::string& avatarAICode,
			   int avatarMaxSP, int avatarMaxHP)
	: id(avatarID), name(avatarName),
	  currentSP(avatarMaxSP), maxSP(avatarMaxSP),
	  currentHP(avatarMaxHP), maxHP(avatarMaxHP),
	  code(avatarAICode)
{
}

Avatar::~Avatar()
{
}

std::string Avatar::getName() const
{
	return name;
}

const std::string& Avatar::getCode() const
{
	return code;
}

int Avatar::getCurrentSP() const
{
	return currentSP;
}

int Avatar::getMaxSP() const
{
	return maxSP;
}

int Avatar::getAvatarID() const
{
	return id;
}

bool Avatar::action(int amount, std::function<bool ()> act_func)
{
	if (currentSP - amount < 0)			// 행동에 필요한 SP가 있는지 검사한다.
		return false;					// 충분한 SP가 없으면 행동에 실패한 것으로 간주한다.

	bool result = act_func();			// 일단 행동에 필요한 조건은 갖추었으므로 행동한다.
	if (result)							// 그리고 행동이 성공했는지 검사한다.
		currentSP -= amount;			// 만약 성공했다면 SP를 차감한다.

	return result;						// 행동의 성공여부를 반환한다.
}

void Avatar::rest()
{
	currentSP = maxSP;
}

int Avatar::getCurrentHP() const
{
	return currentHP;
}

int Avatar::getMaxHP() const
{
	return maxHP;
}

void Avatar::attacked(int amount)
{
	if (currentHP - amount < 0)
		currentHP = 0;
	else
		currentHP -= amount;
}

void Avatar::healed(int amount)
{
	if (currentHP + amount > maxHP)
		currentHP = maxHP;
	else
		currentHP += amount;
}

void Avatar::suiside()
{
	currentHP = 0;
}

bool Avatar::isDead() const
{
	return (currentHP <= 0);
}

} /* namespace AIOnly */
