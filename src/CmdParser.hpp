/*
 * CmdParser.h
 *
 *  Created on: 2013. 11. 17.
 *      Author: hibm98
 */

#ifndef CMDPARSER_HPP_
#define CMDPARSER_HPP_

#include <string>
#include <vector>

namespace AIOnly
{

typedef std::vector<std::string> CmdArgs;
typedef	int						 CmdType;

/*
 *
 */
class CmdParser
{
	CmdArgs cmdArgs;
	CmdType cmdType;

	const int UNKNOWN_CMDTYPE = 0;

protected:
	virtual CmdType	parseCmdType(const std::string& cmd) = 0;

public:
	CmdParser();
	virtual ~CmdParser();

	void parse(const std::string& command);
	CmdType getCmdType() const;
	CmdArgs getArgs() const;
	std::string getArg(int argID) const;
	int getArgSize() const;

};

} /* namespace AIOnly */

#endif /* CMDPARSER_HPP_ */
