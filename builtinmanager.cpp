
#include "builtinmanager.h"

#include "plugins/hist.h"
#include "plugins/echo.h"
#include "plugins/changedir.h"

using namespace std;

BuiltInManager::BuiltInManager(){
    Hist *hist = new Hist();
    commands[hist->getName()] = hist;

    Echo *echo = new Echo();
    commands[echo->getName()] = echo;

    ChangeDir *cd = new ChangeDir();
    commands[cd->getName()] = cd;
}

ErrorCodes BuiltInManager::run(string &name, vector<string> &args){
    if(commands.find(name) != commands.end() ){
        return commands[name]->run(args);
    }
    return NOPROGRAM;
}
