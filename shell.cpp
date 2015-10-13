#include "shell.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <errno.h>
#include <unistd.h>
#include <fstream>
#include <climits>

#include "command.h"
#include "parser.h"
#include "plugins/hist.h"
#include "history.h"

using namespace std;

Shell * Shell::instance = nullptr;

unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    strs.clear();
    istringstream ss(txt);
    string textHolder;
    // Decompose statement
    while(getline(ss, textHolder, ' ')){
        strs.push_back(textHolder);
    }
    return strs.size();
}

Shell* Shell::sharedShell(){
    if (nullptr == instance){
        instance = new Shell();
        instance->manager = new BuiltInManager();
    }
    return instance;
}

Shell::Shell(): workingDir("~"),  commandsRun(1), running(true){
    homeDir = getenv("HOME"); 
    workingDir = homeDir;
    basePID = getpid();
    pwdDidChange();
    string settings = homeDir + "/.yshrc";
    this->settings = buildSettings(settings);
    history.init(&this->settings);
}

Shell::~Shell(){
    delete currentCommand;
}
int Shell::Run(int argc, char** argv){
    sharedShell();
    if(argc < 2)
        return instance->runShell();
    else{
        std::vector<std::string> commands;
        //First arg is name second arg is -c flag
        for(int i = 2; i < argc; i++){
            commands.push_back(argv[i]);
        }
        return instance->runCommand(commands);
    }
}

map<string, string> Shell::buildSettings(string  file){
    map< string, string> settings;
    ifstream settingsFile;
    settingsFile.open(file);
    string key, value;
    while(settingsFile >> key >> value){
        settings[key] = value;
    }
    return settings;
}

int Shell::runShell(){
    while (running){
        string prompt;
        cout << "yshell ran<" << commandsRun << ">: " << workingDir << " >> ";
        vector<string> commands;
        getline(cin, prompt);
        //Has to be done before split or else. . .
        if(cin.eof()){
            cout << endl << "Good Bye" << endl;
            break;
        }
        split(prompt, commands, ' ');
        if (commands.size() < 1) continue;
        if(commands[0] == "quit" ){ 
            running = false;
            prompt = "Good Bye";
        }

        else if(commands[0] == "curPid"){
            cout << "Current PID is " << basePID << endl;
            continue;
        }

        if(manager->run(commands[0], commands) == NOPROGRAM){
            //TODO:Parse command
            vector<Command *> cmds = Parser::BuildCommands(commands);
            for (auto cmd : cmds){
                currentlyRunningCMD = cmd;
                switch (cmd->run()){
                    case (NOPROGRAM):
                        cout << "Error: " << commands[0] << " command not found" << endl;
                        break;
                    case (SEGFAULT):
                        cout << "Segmentation Fault" << endl;
                        break;
                    default:
                        break;
                }
            }
            if(currentlyRunningCMD != nullptr)
                currentlyRunningCMD = nullptr;
        }
        commandsRun ++;
    }
    return 0;
}

void Shell::didRecieveSignal(int signal){
    if(signal == SIGINT){
        if(currentlyRunningCMD != nullptr)
            currentlyRunningCMD->gotSignal(SIGINT);
        else{
            cout << endl;
            if(cin.peek())
                cin.get();
        }
 
    }
}

int Shell::runCommand(vector<string> commands){
  if(manager->run(commands[0], commands) != NOPROGRAM) return 0;
  vector<Command *> cmds = Parser::BuildCommands(commands);
  for (auto cmd : cmds){
    currentlyRunningCMD = cmd;
    switch (cmd->run()){
        case (NOPROGRAM):
            cout << "Error: " << commands[0] << " command not found" << endl;
            break;
        case (SEGFAULT):
            cout << "Segmentation Fault" << endl;
            break;
        default:
            break;
      }
  }
  return 0;
}

void Shell::addToHistory(Command *cmd){
    history.add(cmd);
}

void Shell::displayHist(unsigned int num) {
    for(auto cmd : history.getHistory(num)){
        cout << cmd.first << " ";
        cmd.second->printCommand(cout, false);
        cout << endl;
    }
}

void Shell::pwdDidChange(){
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        workingDir = cwd;
}
