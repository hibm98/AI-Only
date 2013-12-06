/*
 * CLI_main.cpp
 *
 *  Created on: 2013. 11. 3.
 *      Author: hibm98
 */
#include <cstdlib>
#include <iostream>
#include <string>
#include "RoomManager.hpp"
#include "ServerCmdParser.hpp"

inline void FIXME(const char* why)
{
	std::cout << "FIXME : " << why << std::endl;
}

const char* NOT_IMPLEMENT = "This function is not implement.";

int main(void)
{
	using namespace std;
	cout << "AIOnly (Standalone Edition)" << endl
		 << "(C)2013 Kang Jeonggyu (hibm98@gmail.com)" << endl << endl;

	AIOnly::RoomManager rooms;
	AIOnly::ServerCmdParser parser;

	string command;
	bool exit_flag = false;
	int cmdCount = 0;
	int playerNumber = 0;

	while(1)
	{
		// TODO : 수신된 게임 결과가 있는지 검사하는 부분.

		// TODO : 있으면 알려주는 부분.

		// 사용자에게 명령어 입력을 받는 부분.
		cout << ++cmdCount << "> ";
		getline(cin, command);

		if (command.empty())	// 비어있으면 다시 입력칸을 출력.
			continue;

		// 명령을 수행하는 부분.
		// TODO 2.0+ : 함수 부분을 따로 떼내서 실행할 수 있도록 리팩터링 필요.
		parser.parse(command);
		switch (parser.getCmdType())
		{
		case AIOnly::ServerCmdType::CLI_EXIT:
		{
			exit_flag = true;
			break;
		}

		case AIOnly::ServerCmdType::CLI_HELP:
		{
			cout  << "Available command : " << endl
				  << "exit" << endl
				  << "help" << endl
				  << "say" << endl << endl

				  << "* Player command *" << endl
				  << "create_player" << endl
				  << "delete_player" << endl
				  << "player_list" << endl << endl

				  << "* Room command *" << endl
				  << "join_room" << endl
				  << "leave_room" << endl
				  << "room_list" << endl

				  << "* Game command *" << endl
				  << "start_game" << endl
				  << "view_report" << endl;
			break;
		}

		case AIOnly::ServerCmdType::SAY:
		{
			cout << "Server: " << parser.getArg(0) << endl;
			break;
		}

		case AIOnly::ServerCmdType::CREATE_PLAYER:
		{
			if (parser.getArgSize() != 2)
			{
				cout << "Usage: create_player <player name> <player's script file's url>" << endl;
				break;
			}

			if (!rooms.createPlayer(parser.getArg(0), parser.getArg(1), playerNumber + 1))
				cout << "Create player failed! (possible duplicate player name)" << endl;
			else
				cout << "Player " << parser.getArg(0) << " is created! (ID : " << ++playerNumber << ")" << endl;
			break;
		}

		case AIOnly::ServerCmdType::DELETE_PLAYER:
		{
			if (parser.getArgSize() != 1)
			{
				cout << "Usage: delete_player <player name>" << endl;
				break;
			}

			auto exception = rooms.removePlayer(parser.getArg(0));
			if (!exception)
				cout << "Player " << parser.getArg(0) << " is deleted." << endl;
			else
				cout << "Error occured during deleting player " << parser.getArg(0) << "." << endl;

			break;
		}

		case AIOnly::ServerCmdType::PLAYER_LIST:
		{
			if (parser.getArgSize() > 1)
			{
				cout << "Usage: player_list [room id]" << endl;
				break;
			}

			if (!parser.getArgSize())
			{
				AIOnly::PlayersInfoPtr player_list = rooms.getAllPlayerInfo();
				if (player_list->begin() == player_list->end())
				{
					cout << "No created player." << endl;
					break;
				}

				cout << "ID\tName\tSource" << endl;
				for (auto it = player_list->begin(); it != player_list->end(); it++)
				{
					cout << get<AIOnly::PlayerInfo_TupleType::PID>(*it) << "\t"
						 << get<AIOnly::PlayerInfo_TupleType::PNAME>(*it) << "\t"
						 << get<AIOnly::PlayerInfo_TupleType::PSOURCE>(*it) << "\t"
						 << endl;
				}
			}
			else
			{
				// TODO : Implement player_list with room id.
				FIXME(NOT_IMPLEMENT);	// FIXME
			}

			break;
		}

		case AIOnly::ServerCmdType::JOIN_ROOM:
		{
			if (parser.getArgSize() < 1)
			{
				cout << "Usage: join_room <player name> [room id]" << endl;
				break;
			}

			int roomID;
			try {
				roomID = parser.getArgSize() == 2 ? std::stoi(parser.getArg(1)) : 0 ;
			}
			catch (std::exception& e)
			{
				cout << "Invaild Room ID. (" << parser.getArg(1) << ")" << endl;
				break;
			}

			int result = rooms.joinRoom(parser.getArg(0), roomID);
			if (result < 1)
			{
				cout << "Join room failed. (possibly player already joined room)" << endl;
				break;
			}

			cout << "Player " << parser.getArg(0) << " is joined No." << result << " room. " << endl;
			break;
		}

		case AIOnly::ServerCmdType::LEAVE_ROOM:
		{
			if (parser.getArgSize() != 1)
			{
				cout << "Usage: leave_room <player name>" << endl;
				break;
			}

			auto exception = rooms.leaveRoom(parser.getArg(0));
			if (!exception)
				cout << "Leaved." << endl;
			else
				cout << "Error occured during leaving player " << parser.getArg(0) << "." << endl;
			break;
		}

		case AIOnly::ServerCmdType::ROOM_LIST:
		{
			if (parser.getArgSize() > 0)
			{
				cout << "Usage: room_list" << endl;
				break;
			}

			AIOnly::RoomsInfoPtr room_list = rooms.getAllRoomInfo();
			if (room_list->begin() == room_list->end())
			{
				cout << "No created room." << endl;
				break;
			}

			cout << "ID\tStatus\tCurrent\tMax" << endl;
			for (auto it = room_list->begin(); it != room_list->end(); it++)
			{
				cout << get<AIOnly::RoomInfo_TupleType::ROOMID>(*it) << "\t"
					 << get<AIOnly::RoomInfo_TupleType::STATUS>(*it) << "\t"
					 << get<AIOnly::RoomInfo_TupleType::CURRENT>(*it) << "\t"
					 << get<AIOnly::RoomInfo_TupleType::MAX>(*it)
					 << endl;
			}
			break;
		}

		case AIOnly::ServerCmdType::START_GAME:
		{
			// TODO
			break;
		}

		case AIOnly::ServerCmdType::VIEW_REPORT:
		{
			// TODO
			break;
		}

		case AIOnly::ServerCmdType::UNKNOWN:
		default:
		{
			cout << "Unknown command: " << command << endl;
			break;
		}
		}
		cout << endl;

		if (exit_flag)
			break;
	}

	cout << "Good bye!" << endl;
	return EXIT_SUCCESS;
}
