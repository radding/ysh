#include "pipe.h"
#include <stdio.h>
#include <unistd.h>
#include "background.h"

using namespace std;

ErrorCodes Pipe::run(){
    BackgroundCommand *com1 = new BackgroundCommand(cmd1);
    _makeFileDescriptors();
    Command *com2 = cmd2;
    com1->_makeFileDesciptors(fd);
    com2->_makeFileDesciptors(fd);
    com2->_SetFDDir(0);
    com1->_SetFDDir(1);
    com1->run();
    com2->run();
    
    return GOOD;
}

void Pipe::addCmd(Command *cmd){
    DoubleCmd::addCmd(cmd);
}
