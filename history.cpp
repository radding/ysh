#include <fstream>

#include "history.h"

#include "shell.h"
#include "command.h"

using namespace std;

void History::init(map <string, string> *shellSettings) {
    if(shellSettings->find("historyCache") != shellSettings->end())
        cacheSize = stol(shellSettings->find("historyCache")->second);
    if(shellSettings->find("historySize") != shellSettings->end())
        cacheSize = stol(shellSettings->find("historySize")->second);
}

void History::add(Command *cmd){
    pair<unsigned long, Command*> temp(history.size(), cmd);
    history.insert(history.begin(), temp);
    swapHist();
}

std::vector<std::pair<unsigned long, Command *> > History::getHistory(unsigned int number){
    std::vector<std::pair<unsigned long, Command *> > cmds;
    for(unsigned int i = 0; i < number && i < history.size(); i++){
        cmds.push_back(history[i]);
    }
    return cmds;
}

void History::swapHist(){
    if (history.size() < cacheSize + 1) return;
    ofstream history_file;
    string fileName = Shell::sharedShell()->getHomeDir() + ".ysh.hist";
    history_file.open(fileName);
    pair <unsigned long, Command*> temp = history.back();
    history_file << temp.first << ";";
    temp.second->printCommand(history_file, false); 
    history_file << endl;
    history_file.close();
}

void History::loadFromSwap(int number){

}
