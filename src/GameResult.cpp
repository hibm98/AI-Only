/*
 * GameResult.cpp
 *
 *  Created on: 2013. 12. 2.
 *      Author: hibm98
 */

#include "GameResult.hpp"

namespace AIOnly
{

GameResult::GameResult(GameResultCode code, const std::string& whyDraw)
	: result(code), _whyDraw(whyDraw)
{
	// 원래 승자 목록도 이곳 생성자에 만들어두면 좋겠으나,
	// 오류만 내는 경우나 무승부인 경우에 불필요하게 빈 승자목록을 만드는 것은
	// 자원 낭비라고 생각하여 최대한 뒤로 미루게 하였다.
	// 실제로 승자 목록이 생성되는 부분은 addWinner이다.
}

GameResult::~GameResult()
{
}

GameResultCode GameResult::getResultCode() const
{
	return result;
}

const std::string& GameResult::whyDraw() const
{
	return _whyDraw;
}

void GameResult::addWinner(PlayerPtr winner)
{
	// 기존의 승자 목록이 있는지 검사한다. 만약 없으면, 새로 만든다.
	if (!winners)
		winners = std::make_shared<PlayerList>();

	// 승자 목록에 승자를 추가한다.
	winners->insertPlayer(winner);
}

const PlayerListPtr GameResult::getWinners() const
{
	return winners;
}

} /* namespace AIOnly */
