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
	std::string _whyDraw;

public:
	GameResult(GameResultCode code = GameResultCode::PLEASE_SET_RESULTCODE, const std::string& whyDraw = std::string());
	virtual ~GameResult();

	GameResultCode getResultCode() const;
	const std::string& whyDraw() const;

	void addWinner(PlayerPtr winner);
	const PlayerListPtr getWinners() const;
};

} /* namespace AIOnly */

#endif /* GAMERESULT_HPP_ */
