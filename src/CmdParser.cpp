/*
 * CmdParser.cpp
 *
 *  Created on: 2013. 11. 17.
 *      Author: hibm98
 */

#include "CmdParser.hpp"

namespace AIOnly
{

CmdParser::CmdParser()
	: cmdType(UNKNOWN_CMDTYPE)
{
	// TODO Auto-generated constructor stub
}

CmdParser::~CmdParser()
{
	// TODO Auto-generated destructor stub
}

void CmdParser::parse(const std::string& command)
{
	// 매개변수의 초기화를 먼저 한다.
	if (!cmdArgs.empty())
		cmdArgs.clear();

	// TODO 1.1+ : 문장의 앞 뒤 공백을 지운다.

	// 문장에서 첫번째 구분자를 찾는다.
	int sp = command.find_first_of(' ');
	std::string cmd, args;

	// 명령어와 매개변수를 분리한다. 구분자가 없으면 명령어만 있는 것이다.
	if (sp != std::string::npos)
	{
		cmd = command.substr(0, sp);
		args = command.substr(sp+1);
	}
	else
	{
		cmd = command;
	}

	// 앞부분(명령어)을 분석해서 명령어 종류를 알아낸다.
	cmdType = parseCmdType(cmd);

	if (!args.empty())
	{
		// 뒷부분을 구분자로 분리해서 마지막 한 개를 제외한 매개변수에 차례대로 넣는다.
		while((sp = args.find_first_of(' ')) != std::string::npos)
		{
			cmdArgs.push_back(args.substr(0, sp));
			args = args.substr(sp+1);
		}

		// 마지막으로 남은 매개변수도 넣는다.
		cmdArgs.push_back(args);
	}

	// TODO 2.0+ : 연쇄적인 파서 활용을 위해 매개변수 분리 전의 원본 문자열을 제공하는 함수 추가.
}

CmdType CmdParser::getCmdType() const
{
	return cmdType;
}

CmdArgs CmdParser::getArgs() const
{
	return cmdArgs;
}

std::string CmdParser::getArg(int argID) const
{
	if (0 <= argID && argID < cmdArgs.size())
		return cmdArgs.at(argID);
	else
		return "";
}

int CmdParser::getArgSize() const
{
	return cmdArgs.size();
}

} /* namespace AIOnly */
