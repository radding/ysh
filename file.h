#pragma once
#include "command.h"

class Right : public Command
{
public:
  Right(std::string &file);
  virtual ErrorCodes run() override;

private:
  std::string f;
};
