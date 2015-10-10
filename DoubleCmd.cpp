#include "DoubleCmd.h"

using namespace std;
ErrorCodes DoubleCmd::run()
{
    currentCmd = cmd1;
    ErrorCodes cmd1Status = cmd1->run();
    if(cmd1Status == GOOD){
        currentCmd = cmd2;
        return cmd2->run();
    }
    return cmd1Status;
}

void DoubleCmd::addCmd(Command * cmd)
{
    if(cmd1 == nullptr) cmd1 = cmd;
    else if(cmd2 == nullptr) cmd2 = cmd;
}
