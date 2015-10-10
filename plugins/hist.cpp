
#include "hist.h"
#include "../shell.h"

using namespace std;
ErrorCodes Hist::run(vector <string> &args){
    if (args.size() > 1){
        shell()->displayHist(stoi(args[1].c_str()));
    }
    else shell()->displayHist(10);
    return Builtin::ran(args);
}

Hist::Hist(){
    commandName = "hist";
}

