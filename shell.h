
#pragma once
#include <string>
#include <vector>
#include <signal.h>

#include "history.h"
#include "builtin.h"
#include "builtinmanager.h"

class Command;

class Shell
{
private:
    static Shell *instance;
    Command *currentCommand;
    History history;
    std::string pid;
    std::string workingDir;
    long commandsRun;
    bool running;
    std::string homeDir;
    std::map <std::string, std::string> settings;
    std::map <std::string, std::string> buildSettings(std::string file);
    BuiltInManager *manager;
    Command *currentlyRunningCMD;

protected:
    Shell();
    ~Shell();

public:
    pid_t basePID;
    int runShell();
    int runCommand(std::vector<std::string>);

    static Shell* sharedShell();
    
    void addToHistory(Command *);
    int Run(int argc, char** argv);

    std::string getHomeDir() { return homeDir; }
    void pwdDidChange();
    void displayHist(unsigned int);
    void didRecieveSignal(int signal);
};
