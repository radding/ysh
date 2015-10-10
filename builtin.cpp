
#include "shell.h"
#include "builtin.h"

using namespace std;

Shell *Builtin::shell(){
    return Shell::sharedShell();
}

Builtin::Builtin(){

}

ErrorCodes Builtin::ran(vector<string> &args) {
    if(args.size() > 1)
        args.erase(args.begin());
    this->args = args;
    shell()->addToHistory(this);
    return GOOD;
}

