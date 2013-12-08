/*
 * Game.cpp
 *
 *  Created on: 2013. 11. 3.
 *      Author: hibm98
 */

#include "Game.hpp"

namespace AIOnly
{

Game::Game(const PlayerListPtr players)
	: _players(players)
{
}

Game::~Game()
{
}

void Game::prepare()
{
	logger.echoLog(true);
	std::ostringstream logt;

	// 게임에 참가할 플레이어들의 정보를 먼저 얻어온다.
	PlayersInfoPtr players = _players->getPlayersInfo();

	for(PlayerInfo t : *players)
	{
		logt << std::get<PlayerInfo_TupleType::PNAME>(t) << "(" << std::to_string(std::get<PlayerInfo_TupleType::PID>(t)) << ")님이 게임에 참가하였습니다.";
		logger.write(LogLevel::INFO, logt.str());
		logt.str("");
	}

	// 실행 순서를 무작위로 결정한 뒤 순서에 따라 플레이어 정보를 큐에 저장
	// 순서를 정하기 위한 난수 발생기를 준비한다.
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> pos(0, 9);

	// 플레이어 정보 목록에서 무작위로 뽑아서 나온 플레이어의 정보를 토대로 아바타로 만든 뒤 순서 큐에 집어넣는다.
	while (players->size() > 0)
	{
		// NOTE : 음... 나중에 players.size()가 0인지 검사하는 부분이 필요할 것 같다.
		std::uniform_int_distribution<int> dis(0, players->size()-1);	// 범위 : 플레이어의 인원수 이내
		int selected = dis(gen);

		PlayerPtr player = _players->getPlayer(std::get<PlayerInfo_TupleType::PID>(players->at(selected)));
		AvatarPtr avatar = std::make_shared<Avatar>(player->getID(), player->getName(), player->getCode());

		// 맵에 배치될 때까지 무한 시도 (사실 2명 뿐이니까 가능한 시도임!)
		while(!map.allocateAvatar(pos(gen), pos(gen), avatar));
		avatar_queue.push_back(avatar);
		players->erase(players->begin() + selected);

		logt << avatar_queue.size() << "번째로 실행될 " << player->getName() << "(" << player->getID() << ")님의 아바타가 생성되었습니다. (ID=" << avatar->getAvatarID() << ")";
		logger.write(LogLevel::INFO, logt.str());
		logt.str("");
	}
}

void Game::ingame()
{
	Lua	engine;
	LuaTable global = engine.GetGlobalEnvironment();
	AvatarPtr current;
	std::chrono::seconds onestep(1);
	std::ostringstream logt;

	// ***** 함수를 정의하는 단계. *****
	auto walk_func = [&](int direction) -> int
		{
			Direction tmp;
			std::string dirs;

			switch(direction)
			{
			case 0:
				tmp = Direction::NORTH;
				dirs = "북쪽";
				break;
			case 1:
				tmp = Direction::EAST;
				dirs = "동쪽";
				break;
			case 2:
				tmp = Direction::SOUTH;
				dirs = "남쪽";
				break;
			case 3:
			default:
				tmp = Direction::WEST;
				dirs = "서쪽";
				break;
			};

			logt << current->getName() << "(" << current->getAvatarID() << ")이/가 " << dirs << "으로 이동을 시도합니다.";
			logger.write(LogLevel::INFO, logt.str());
			logt.str("");

			return (int)map.walk(current, tmp);
		};

	auto attack_func = [&](int direction) -> int
		{
			Direction tmp;
			std::string dirs;

			switch(direction)
			{
			case 0:
				tmp = Direction::NORTH;
				dirs = "북쪽";
				break;
			case 1:
				tmp = Direction::EAST;
				dirs = "동쪽";
				break;
			case 2:
				tmp = Direction::SOUTH;
				dirs = "남쪽";
				break;
			case 3:
			default:
				tmp = Direction::WEST;
				dirs = "서쪽";
				break;
			};

			logt << current->getName() << "(" << current->getAvatarID() << ")이/가 " << dirs << "으로 공격을 시도합니다.";
			logger.write(LogLevel::INFO, logt.str());
			logt.str("");

			return (int)map.attack(current, tmp);
		};

	auto suiside_func = [&]() -> void
		{
			logt << current->getName() << "(" << current->getAvatarID() << ")이/가 자살합니다...";
			logger.write(LogLevel::INFO, logt.str());
			logt.str("");

			current->suiside();
		};

	// ***** 함수를 등록하는 단계. *****
	auto lua_walkfunc = engine.CreateFunction<int(int)>(walk_func);
	auto lua_attackfunc = engine.CreateFunction<int(int)>(attack_func);
	auto lua_suisidefunc = engine.CreateFunction<void()>(suiside_func);

	global.Set("walk", lua_walkfunc);
	global.Set("attack", lua_attackfunc);
	global.Set("suiside", lua_suisidefunc);

	// ***** 최초 실행 준비 *****
	logger.write(LogLevel::INFO, "게임 준비가 완료되었습니다. 게임을 시작합니다.");

	// 이제 게임을 시작하기 위해 이터레이터를 생성한다.
	auto it = avatar_queue.begin();
	int count = 100 * avatar_queue.size();	// 턴 제한횟수

	logt << "게임에서 진행할 수 있는 최대 턴수는 " << count << "턴입니다.";
	logger.write(LogLevel::INFO, logt.str());
	logt.str("");

	// 이제 순서에 따라 코드를 실행한다. (게임이 끝날 때까지 무한 반복)
	while (true)
	{
		// 턴 카운터가 0이면 무승부로 결정짓는다.
		if (count-- <= 0)
		{
			logger.write(LogLevel::INFO, "최대 턴수에 도달하였습니다. 게임을 중단하곘습니다.");
			whyDraw = "Reached count limit(100)";
			break;
		}

		current = (*it);

		logt << "** 현재 " << current->getName() << "(" << current->getAvatarID() << ")의 턴입니다. **";
		logger.write(LogLevel::INFO, logt.str());
		logt.str("");

		//std::this_thread::sleep_for(onestep);

		// ***** 코드를 실행하는 단계. *****
		std::string result = engine.RunScriptWithHook(current->getCode(), 1000);
		if (result != "No errors")
		{
			// 일단 정상적으로 끝난것이 아니다.
			// 실행 횟수 초과로 끝난 것이 아닌지(컴파일 에러 등...) 판단하여 아니라면 아바타를 자살시켜버린다.
			if (result != "Error: Time exceed.\n")
			{
				logger.write(LogLevel::ERR, "현재 아바타의 코드에 문제가 있어서 행동할 수 없습니다!");
				logger.write(LogLevel::ERR, result);

				current->suiside();
				logger.write(LogLevel::ERR, "해당 아바타는 플레이어를 원망하면서 자폭하였습니다.");
			}
			else
			{
				logger.write(LogLevel::WARN, "현재 아바타의 코드는 제한된 수보다 많은 연산을 수행하려고 하였습니다.");
				logger.write(LogLevel::WARN, "현재 아바타의 코드 실행을 중단하고 계속 게임을 진행합니다.");
			}
		}

		// ***** 다음 순서를 결정하는 단계. ******
		// 먼저 죽은 아바타가 있으면 지워준다.
		for (auto chkDeadIt = avatar_queue.begin(); chkDeadIt != avatar_queue.end(); chkDeadIt++)
		{
			if ((*chkDeadIt)->isDead())
			{
				logt << "사망함: " << (*chkDeadIt)->getName() << "(" << (*chkDeadIt)->getAvatarID() << ")";
				logger.write(LogLevel::INFO, logt.str());
				logt.str("");

				chkDeadIt = avatar_queue.erase(chkDeadIt);
			}
		}

		// 만약 남은 아바타가 한 명 이하라면 게임을 종료한다.
		if (avatar_queue.size() <= 1)
			break;

		// 이제 다음 아바타로 넘어간다.
		++it;
		if (it == avatar_queue.end())
			it = avatar_queue.begin();
	}
}

void Game::cleanup()
{
	std::ostringstream logt;

	// 최후의 아바타를 가져온다.
	if (avatar_queue.size() == 1)
	{
		result = GameResult(GameResultCode::WIN);
		result.addWinner(_players->getPlayer(avatar_queue[0]->getAvatarID()));
	}
	else
	{
		result = GameResult(GameResultCode::DRAW, whyDraw);
	}

	// 게임 기록을 DB에 저장.
	if (result.getResultCode() == GameResultCode::WIN)
		logt << "Winner : " << avatar_queue[0]->getName();
	else
		logt << "Draw... (" << result.whyDraw() << ")";


	logger.write(LogLevel::INFO, logt.str());
	logt.str("");
}

void Game::run()
{
	prepare();
	ingame();
	cleanup();
}

const GameResult& Game::getResult() const
{
	return result;
}

} /* namespace AIOnly */
