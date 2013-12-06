/*
 * MapTile.hpp
 *
 *  Created on: 2013. 12. 3.
 *      Author: hibm98
 */

#ifndef MAPTILE_HPP_
#define MAPTILE_HPP_

#include <memory>
#include "Avatar.hpp"

namespace AIOnly
{

enum class TileType
{
	DIRT,
	SAND,
	ROAD
};

struct AvatarUniquePtrDeleter
{
	void operator() (Avatar* p) const {}	// Nothing
};

typedef std::unique_ptr<Avatar, AvatarUniquePtrDeleter> AvatarUniquePtr;

/*
 *
 */
class MapTile
{
	TileType _type;
	AvatarUniquePtr _avatar;

public:
	MapTile(TileType type = TileType::DIRT);
	virtual ~MapTile();

	void attacked(int amount);
	void healed(int amount);

	TileType getTileType() const;
	bool hasAvatar() const;
	const Avatar getEnteredAvatar() const;

	// Helper method
	friend bool ::AIOnly::allocateAvatar(MapTile& destination, const AvatarPtr& target);
	friend bool ::AIOnly::moveAvatar(MapTile& source, MapTile& destination);
};

bool allocateAvatar(MapTile& destination, const AvatarPtr& target);
bool moveAvatar(MapTile& source, MapTile& destination);

} /* namespace AIOnly */

#endif /* MAPTILE_HPP_ */
