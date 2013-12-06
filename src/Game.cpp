/*
 * Game.cpp
 *
 *  Created on: 2013. 11. 3.
 *      Author: hibm98
 */

#include "Game.hpp"

#include <fstream>
#include <ctime>
#include <locale>

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
	// 게임에 참가할 플레이어들의 정보를 먼저 얻어온다.
	PlayersInfoPtr players = _players->getPlayersInfo();

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
	}
}

void Game::ingame()
{
	Lua	engine;
	LuaTable global = engine.GetGlobalEnvironment();
	AvatarPtr current;
	std::chrono::seconds onestep(1);

	// ***** 함수를 정의하는 단계. *****
	auto walk_func = [&](int direction) -> int
		{
			Direction tmp;
			switch(direction)
			{
			case 0:
				tmp = Direction::NORTH;
				break;
			case 1:
				tmp = Direction::EAST;
				break;
			case 2:
				tmp = Direction::SOUTH;
				break;
			case 3:
				tmp = Direction::WEST;
				break;
			default:
				tmp = Direction::NORTH;
			};

			return (int)map.walk(current, tmp);
		};

	auto attack_func = [&](int direction) -> int
		{
			Direction tmp;
			switch(direction)
			{
			case 0:
				tmp = Direction::NORTH;
				break;
			case 1:
				tmp = Direction::EAST;
				break;
			case 2:
				tmp = Direction::SOUTH;
				break;
			case 3:
				tmp = Direction::WEST;
				break;
			default:
				tmp = Direction::NORTH;
			};

			return (int)map.attack(current, tmp);
		};

	// ***** 함수를 등록하는 단계. *****
	auto lua_walkfunc = engine.CreateFunction<int(int)>(walk_func);
	auto lua_attackfunc = engine.CreateFunction<int(int)>(attack_func);

	global.Set("walk", lua_walkfunc);
	global.Set("attack", lua_attackfunc);

	// ***** 최초 실행 준비 *****
	// 이제 게임을 시작하기 위해 이터레이터를 생성한다.
	auto it = avatar_queue.begin();

	// 이제 순서에 따라 코드를 실행한다. (게임이 끝날 때까지 무한 반복)
	while (true)
	{
		current = (*it);
		std::this_thread::sleep_for(onestep);

		// ***** 코드를 실행하는 단계. *****
		std::string result = engine.RunScriptWithHook(current->getCode(), 1000);

		// ***** 다음 순서를 결정하는 단계. ******
		// 먼저 죽은 아바타가 있으면 지워준다.
		for (auto chkDeadIt = avatar_queue.begin(); chkDeadIt != avatar_queue.end(); chkDeadIt++)
		{
			if ((*chkDeadIt)->isDead())
				chkDeadIt = avatar_queue.erase(chkDeadIt);
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
	// 최후의 아바타를 가져온다.
	result = GameResult(GameResultCode::WIN);
	result.addWinner(_players->getPlayer(avatar_queue[0]->getAvatarID()));

	// 게임 기록을 DB에 저장.
	std::locale::global(std::locale("ko_KR.utf8"));
	std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char now_time[100];
	std::strftime(now_time, 100, "%c", std::localtime(&t));

	std::ofstream fout("Result_" + std::string(now_time) + ".txt");
	fout << "Winner : " << avatar_queue[0]->getName() << std::endl;
	fout.close();
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
