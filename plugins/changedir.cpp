#include "changedir.h"
#include <errno.h>
#include <unistd.h>

#include "../shell.h"
using namespace std;

ChangeDir::ChangeDir(){
    commandName = "cd";
}

ErrorCodes ChangeDir::run(std::vector <std::string> &thing) {
    string path = shell()->getHomeDir();
    if (thing.size() > 1)
        path = thing[1];
    int returnValue = chdir(path.c_str());
    if(returnValue < 0){
        switch (errno){
            case (ENOENT):
                cout << "Directory " << path << " does not exsist." << endl;
                break;
            case (ENOTDIR):
                cout << path << " does not name a directory." <<endl;
                break;
            default:
                break;
        }
    }
    else shell()->pwdDidChange();
    return Builtin::ran(thing);
}
