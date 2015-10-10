
#pragma once

#include "../builtin.h"

class Hist : public Builtin {
public:
    virtual ErrorCodes run(std::vector <std::string> &) override;
    Hist();
};
