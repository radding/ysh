#pragma once

#include "builtin.h"
#include <map>

class BuiltInManager {
private:
    std::map<std::string, Builtin *> commands;
public:
    BuiltInManager();
    ErrorCodes run(std::string &, std::vector<std::string> &);
};
