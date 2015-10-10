/**
* \author Yoseph Radding
* \file command.h
*
* Defines a command class
*/

#pragma once
#include <vector>
#include <string>
#include <ostream>

#include "parser.h"

enum ErrorCodes {
    NOPROGRAM = 2,
    GOOD = 0,
    SEGFAULT = 139
};

/**
* Command class defines 
*/
class Command
{

public:
	Command(int, char **){};
	Command(Parser::Node);
    Command(std::string, std::vector<std::string>);
	Command(): runForeground(true) {};
	virtual ~Command();
	void addArg(std::string arg){ args.push_back(arg); }
    void setName(std::string name) { commandName = name; }
    void printCommand(std::ostream &, bool);
    std::string getName() { return commandName; }
    void setRunForeground(bool run) { runForeground = run; }
    virtual ErrorCodes run();
    virtual void gotSignal(int);

protected:
    virtual ErrorCodes _runParentProcess(pid_t childPid);
    virtual ErrorCodes _runChildProcess(char **argv);
	std::string commandName;
	std::vector<std::string > args;
    ErrorCodes status;
    bool runForeground;
private:
    pid_t childPID;
};


