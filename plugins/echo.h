
#pragma once

#include "../builtin.h"

class Echo : public Builtin{
public:
    virtual ErrorCodes run(std::vector <std::string> &) override;
    Echo();
};
