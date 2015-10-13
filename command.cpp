
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

void Command::_makeFileDescriptors(){
    int x[2];
    pipe(x);
    _makeFileDesciptors(x);
}

void Command::_SetFDDir(int dir){
    direction = dir;
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
    if(hasFD){
    }
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
    int STDIN;
    int STDOUT;
    //if(direction == 0)
    //   STDIN = dup(STDIN_FILENO);
    //if(direction == 1)
    //   STDOUT = dup(STDOUT_FILENO);
    if(hasFD) {
        cout << direction << endl;
        dup2(fd[direction], direction);
    }
    if (execvp(argv[0], argv) < 0){
        exitCode = static_cast<ErrorCodes>(errno);
    }
    //if(hasFD){
    //  if(direction == 0)
    //    dup2(STDIN, 0);
    //  if(direction == 1)
    //    dup2(STDOUT, 1);
    //}
    return exitCode;
}

void Command::gotSignal(int sig){
    kill(childPID, sig);
}


void Command::_makeFileDesciptors(int *fds){
    fd[0] = fds[0];
    fd[1] = fds[1];
    hasFD = true;
}
