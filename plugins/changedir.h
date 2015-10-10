
#pragma once
#include "../builtin.h"

class ChangeDir : public Builtin {
public:
    ChangeDir();
    virtual ErrorCodes run(std::vector <std::string> &thing) override;
};
