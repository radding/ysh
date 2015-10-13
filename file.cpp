
#include "file.h"

#include <unistd.h>
#include <fcntl.h>

using namespace std;

Right::Right(string &fileName){
  f = fileName;
}

ErrorCodes Right::run() {
  int fl = open(f.c_str(), O_WRONLY | O_TRUNC | O_CREAT);
  fd[0] = 0;
  fd[1] = fl;
  hasFD = true;
  _SetFDDir(1);
  cout << f << endl;
  cout << commandName << endl;
  return Command::run();
}
