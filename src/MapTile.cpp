/*
 * MapTile.cpp
 *
 *  Created on: 2013. 12. 3.
 *      Author: hibm98
 */

#include "MapTile.hpp"

namespace AIOnly
{

MapTile::MapTile(TileType type)
	: _type(type)
{
}

MapTile::~MapTile()
{
}

void MapTile::attacked(int amount)
{
	if (!_avatar)
		return;

	_avatar->attacked(amount);
}

void MapTile::healed(int amount)
{
	if (!_avatar)
		return;
	else
		_avatar->healed(amount);
}

TileType MapTile::getTileType() const
{
	return _type;
}

bool MapTile::hasAvatar() const
{
	return ((bool)_avatar);
}

const Avatar MapTile::getEnteredAvatar() const
{
	return *(_avatar.get());
}



// Helper method
bool moveAvatar(MapTile& source, MapTile& destination)
{
	if(!source._avatar || destination._avatar)
		return false;

	destination._avatar = std::move(source._avatar);
	return true;
}


bool allocateAvatar(MapTile& destination, const AvatarPtr& target)
{
	if(destination._avatar)
		return false;

	AvatarUniquePtr temp(target.get(), AvatarUniquePtrDeleter());
	destination._avatar = std::move(temp);
	return true;
}

} /* namespace AIOnly */
