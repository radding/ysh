# Author: Yoseph Radding
# CSE 410 Project 2 makefile
#
CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
DEBUG=-g
SOURCES=main.cpp\
	   	parser.cpp\
	   	command.cpp\
	  	shell.cpp\
	   	history.cpp\
	   	builtin.cpp\
	   	plugins/hist.cpp\
	   	builtinmanager.cpp\
		plugins/echo.cpp\
	   	plugins/changedir.cpp\
		background.cpp\
		DoubleCmd.cpp\
		pipe.cpp\
		file.cpp

OBJDIR=.build
OBJECTS=$(patsubst %.cpp, $(OBJDIR)/%.o, $(SOURCES))
EXECUTABLE=PROG2


all: $(SOURCES) $(EXECUTABLE)

debug: CFLAGS += $(DEBUG)
debug: all

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	find . -type f -name '*.o' -delete
