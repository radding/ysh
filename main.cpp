/**
* \Author: Yoseph Radding
* \file main.cpp
*
*  the entry point for the PROG2 program.
*/

#include <iostream>
#include <signal.h>

#include "shell.h"

using namespace std;

void intHandler(int sig){
    Shell::sharedShell()->didRecieveSignal(sig);
}

int main(int argc, char** argv){
    Shell *shell = Shell::sharedShell();
    cout << "YShell version 1.0"  <<endl;
    signal(SIGINT, intHandler);
	return shell->Run(argc, argv);
}
