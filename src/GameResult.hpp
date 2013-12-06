/*
 * GameResult.hpp
 *
 *  Created on: 2013. 12. 2.
 *      Author: hibm98
 */

#ifndef GAMERESULT_HPP_
#define GAMERESULT_HPP_

#include <list>
#include "Player.hpp"
#include "PlayerList.hpp"

namespace AIOnly
{

enum class GameResultCode
{
	DRAW,
	WIN,
	COMPILE_ERROR,
	RUNTIME_ERROR,
	OTHERWISE_ERROR,
	PLEASE_SET_RESULTCODE
};

/*
 *
 */
class GameResult
{
	GameResultCode result;
	PlayerListPtr winners;

public:
	GameResult(GameResultCode code = GameResultCode::PLEASE_SET_RESULTCODE);
	virtual ~GameResult();

	GameResultCode getResultCode() const;

	void addWinner(PlayerPtr winner);
	const PlayerListPtr getWinners() const;
};

} /* namespace AIOnly */

#endif /* GAMERESULT_HPP_ */
