/*
 * Avatar.hpp
 *
 *  Created on: 2013. 11. 3.
 *      Author: hibm98
 */

#ifndef AVATAR_HPP_
#define AVATAR_HPP_

#include <memory>
#include <functional>
#include <string>

namespace AIOnly
{

/*
 *
 */
class Avatar
{
	int id;
	std::string name;
	std::string code;

	int currentSP;
	int maxSP;

	int currentHP;
	int maxHP;

public:
	Avatar(int avatarID,
		   const std::string& avatarName,
		   const std::string& avatarAICode,
		   int avatarMaxSP = 5, int avatarMaxHP = 100);
	virtual ~Avatar();

	std::string getName() const;

	bool isDead() const;
	const std::string& getCode() const;

	// behavior
	bool action(int amount, std::function<bool ()> act_func);

	void attacked(int amount);
	void healed(int amount);
	void suiside();
	void rest();

	// getter
	int getCurrentSP() const;
	int getCurrentHP() const;
	int getMaxSP() const;
	int getMaxHP() const;
	int getAvatarID() const;
};

typedef std::shared_ptr<Avatar> AvatarPtr;

} /* namespace AIOnly */

#endif /* AVATAR_HPP_ */

