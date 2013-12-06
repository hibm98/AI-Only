/*
 * GameMap.cpp
 *
 *  Created on: 2013. 12. 3.
 *      Author: hibm98
 */

#include "GameMap.hpp"

namespace AIOnly
{

GameMap::GameMap()
{
}

GameMap::~GameMap()
{
}

bool GameMap::allocateAvatar(int x, int y, const AvatarPtr& target)
{
	// 범위 검사하기.
	if (!isInnerCoord(x, y))
		return false;

	// 맵에 배치하기.
	if(!::AIOnly::allocateAvatar(_map[y][x], target))
		return false;

	avatar_position[target] = Coord(x, y);
	return true;
}

// 아바타의 위치를 기반으로 최종 목표지점의 maptile에 walk를 할 수 있도록 한다.
bool GameMap::walk(AvatarPtr& avatar, Direction direction, int distance)
{
	// 아바타가 존재하면서 자신의 좌표목록에 등록되어 있는지 검사한다.
	if (!avatar || (avatar_position.find(avatar) == avatar_position.end()))
		return false;

	// 제대로 된 거리인지 검사한다.
	if (distance <= 0)
		return false;

	// 아바타에 필요한 양의 SP가 있는지 검사한다.
	if (avatar->getCurrentSP() < SP_WALK * distance)
		return false;

	// 목적지의 좌표를 구해서 이동할 수 있는 좌표인지 검사한다.
	Coord op(avatar_position[avatar]);
	Coord dp = op;
	for(int i = 0; i < distance; i++)
		dp.move(direction);
	if(!isInnerCoord(dp))
		return false;

	// TODO 2.0+ : 목적지로 가는 중간에 장애물이 있는 타일이 존재하는지 검사한다.

	// 올바르게 옮기기 위한 검사가 모두 끝났다.
	// 이제 아바타를 옮기고, 자신의 좌표목록도 새로운 값으로 고친다.
	if(!::AIOnly::moveAvatar(_map[op.y][op.x], _map[dp.y][dp.x]))
		return false;

	avatar_position[avatar].x = dp.x;
	avatar_position[avatar].y = dp.y;
	return true;
}

// 아바타의 위치를 기반으로 최종 목표지점의 maptile에서 attacked를 할 수 있도록 한다.
bool GameMap::attack(AvatarPtr& avatar, Direction direction, int affect_distance, int amount)
{
	// FIXME 1.1+ : 영향거리를 제대로 반영하도록 구현. (여기선 무조건 1로 간주)
	affect_distance = 1;

	// 아바타가 존재하면서 자신의 좌표목록에 등록되어 있는지 검사한다.
	if (!avatar || (avatar_position.find(avatar) == avatar_position.end()))
		return false;

	// 제대로 된 거리인지 검사한다.
	if (affect_distance <= 0)
		return false;

	// 공격할 수 있는 충분한 SP가 있는지 검사한다.
	if (avatar->getCurrentSP() < SP_ATTACK * affect_distance)
		return false;

	// 공격 범위가 존재하는지 검사한다. (범위가 1인 경우만 가정)
	Coord op(avatar_position[avatar]);
	for(int i = 0; i < affect_distance; i++)
		op.move(direction);
	if(!isInnerCoord(op))
		return false;

	// 공격한다!
	_map[op.y][op.x].attacked(amount);

	return true;
}

const MapTile& GameMap::getTile(int x, int y) const
{
	return _map[y][x];
}

} /* namespace AIOnly */
