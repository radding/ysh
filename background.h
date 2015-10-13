#pragma once

#include "command.h"

class BackgroundCommand : public Command
{
public:
    virtual ErrorCodes _runParentProcess(pid_t childPid) override { return GOOD; }
    BackgroundCommand(Command *cmd);
    BackgroundCommand(){} 
};
