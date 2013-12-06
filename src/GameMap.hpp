/*
 * GameMap.hpp
 *
 *  Created on: 2013. 12. 3.
 *      Author: hibm98
 */

#ifndef GAMEMAP_HPP_
#define GAMEMAP_HPP_

#include <map>
#include <array>
#include <memory>

#include "MapTile.hpp"

namespace AIOnly
{

enum class Direction
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

struct Coord
{
	int x;
	int y;

	Coord(int m_x = 0, int m_y = 0)
		: x(m_x), y(m_y)
	{}

	void move(Direction d)
	{
		if 		(d == Direction::NORTH)	y--;
		else if (d == Direction::EAST)	x++;
		else if (d == Direction::SOUTH)	y++;
		else if (d == Direction::WEST)	x--;
	}
};

/*
 *
 */
class GameMap
{
	class AvatarPtrCompareByID
	{
	public:
		bool operator()(const AvatarPtr& lhs, const AvatarPtr& rhs) const
		{
			return lhs->getAvatarID() < rhs->getAvatarID();
		}
	};

	// Map
	const static int MAP_X_SIZE = 10;
	const static int MAP_Y_SIZE = 10;
	std::array<std::array<MapTile, MAP_X_SIZE>, MAP_Y_SIZE> _map;

	// Avatar Position
	std::map<AvatarPtr, Coord, AvatarPtrCompareByID> avatar_position;

	// SP usage
	const static int SP_WALK = 1;
	const static int SP_ATTACK = 1;

	inline bool isInnerCoord(int x, int y) { return !((x < 0 || x >= MAP_X_SIZE) || (y < 0 || y >= MAP_Y_SIZE)); }
	inline bool isInnerCoord(Coord c) { return this->isInnerCoord(c.x, c.y); }

public:
	GameMap();
	virtual ~GameMap();

	// Avatar Modifier
	//bool allocateAvatar(const AvatarPtr& target, int team); // TODO 3.0+
	bool allocateAvatar(int x, int y, const AvatarPtr& target);

	// Behavior
	bool walk(AvatarPtr& avatar, Direction direction, int distance = 1);
	bool attack(AvatarPtr& avatar, Direction direction, int affect_distance = 1, int amount = 1);

	// Getter
	const MapTile& getTile(int x, int y) const;
};

} /* namespace AIOnly */

#endif /* GAMEMAP_HPP_ */
