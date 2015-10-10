
#pragma once

#include "command.h"

class Shell;

class Builtin : public Command {
public:
    //Builtin(std::string name, std::vector<std::string> args) { Command::Command(name, args); }
    Builtin();
    virtual ErrorCodes run(std::vector <std::string> &thing) { return NOPROGRAM;}
    virtual ErrorCodes run() override {return NOPROGRAM;}
    std::string getName() {return commandName;}
    void init();
protected:
    Shell *shell();
    ErrorCodes ran(std::vector<std::string>&);
};
