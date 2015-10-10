
#include "echo.h"

using namespace std;

ErrorCodes Echo::run(vector<string> &args){
    if (args.size() > 1){
        cout << args[1];
        string quotes = " ";
        for (auto i = args.begin() + 2; i != args.end(); i ++){
            cout <<  quotes << *i;
        }
    }
    cout << endl;
    return Builtin::ran(args);
}


Echo::Echo(){
    commandName = "echo";
}
