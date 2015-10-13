
#pragma once
#include "command.h"

class DoubleCmd : public Command
{
public:
    DoubleCmd(){ Command::setType("double");  }
    virtual void addCmd(Command *);
    bool cmdsFilled() { return (cmd1 != nullptr) && (cmd2 != nullptr); } 
    virtual ErrorCodes run() override;
    virtual void gotSignal(int sig) override { currentCmd->gotSignal(sig); }

protected:
    Command *cmd1 = nullptr;
    Command *cmd2 = nullptr;
    Command *currentCmd = nullptr;
};
