#include "background.h"


BackgroundCommand::BackgroundCommand(Command *cmd){
    args = cmd->getArgs();
    commandName = cmd->getName();
}
