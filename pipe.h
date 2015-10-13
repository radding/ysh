
#pragma once

#include "DoubleCmd.h"

class Pipe : public DoubleCmd
{
public:
    virtual ErrorCodes run() override;
    virtual void addCmd(Command *) override;
};
