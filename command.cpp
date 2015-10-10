
#include "command.h"
#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

#include "shell.h"

using namespace std;

Command::Command(string commandName, vector<string> args) {
    this->commandName = commandName;
    this->args = args;   
}

Command::~Command(){
}

void Command::printCommand(ostream &stream, bool debug)
{
    stream << commandName;
    if(debug)
    {
        stream << endl << "\tArguments:" << endl;
        for (auto arg : args)
        {
            stream << "\t\t " << arg << endl;
        }
    }
    else{
        for(auto arg : args)
        {
            stream << " " << arg;
        }
    }
}

ErrorCodes Command::run(){
    Shell *shell = Shell::sharedShell();
    childPID = fork();
    status = GOOD;
    if(childPID > 0){
        status = this->_runParentProcess(childPID);
        shell->addToHistory(this);
    }
    else if(childPID == 0){
        // first convert the stuff arg vector to an char ** array then call _runChildProcess
        size_t size = args.size()+2;
        char *argv[size];
        std::strcpy (argv[0], commandName.c_str());
        size_t i;
        for(i = 0; i < args.size(); i ++){
            argv[i+1] = new char[args[i].size()];
            std::strcpy (argv[i+1], args[i].c_str());
        }
        i++;
        argv[i] = (char *)NULL;

        //I want the process to kill itself when done. so call exit on the ret value of the child
        exit(_runChildProcess(argv));
    }
    else{
        //handle fork error
        cout << "Error On Fork" << endl;
    }
    return status;
}

ErrorCodes Command::_runParentProcess(pid_t childPid){
    int status;
    //Wait for child to end.
    while(-1 == waitpid(childPid, &status, 0)) {
    }
    //add to history.
    ErrorCodes retVal = GOOD;
    switch(status){
        case(SEGFAULT):
            retVal = SEGFAULT;
            break;
        case(GOOD):
            break;
        case(11):
            retVal = SEGFAULT;
            break;
        default:
            retVal = NOPROGRAM;
            break;
    }
    return retVal;
}

ErrorCodes Command::_runChildProcess(char **argv){
    ErrorCodes exitCode = GOOD;
    if (execvp(argv[0], argv) < 0){
        exitCode = static_cast<ErrorCodes>(errno);
    }
    return exitCode;
}

void Command::gotSignal(int sig){
    kill(childPID, sig);
}
