#pragma once

#include <map>
#include <vector>
#include <utility>
#include <string>

class Command;

class History {
public:
    void add(Command *);
    History() { cacheSize = 10; historySize = 100; }
    void init(std::map <std::string, std::string> *);
    std::vector<std::pair<unsigned long, Command *> > getHistory(unsigned int);
    Command * lastCommand(std::string);
protected:
    void swapHist();
    void loadFromSwap(int);
private:
    std::vector<std::pair<unsigned long, Command *> > history;
    unsigned long cacheSize;
    unsigned long historySize;
};
