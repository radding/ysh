#pragma once

#include <iostream>
#include <string>
#include <vector>

class Command;

enum TokenType {
    CMD = 0,
    ARG = 1,
    PIPE = 2,
    LEFT = 3,
    RIGHT = 4,
    BACKGROUND=5,
    TWO=6
};

class Parser
{
private:
    struct Token{
        std::string value;
        TokenType type;
    };
public:
    struct Node{
        Token token;
        std::vector<Node *> children;
        Node *parent;
    };

    ///Default Constructor (Delete)
	Parser() = delete;
    //Copy Constructor. Disabled
	~Parser();
    Parser(std::vector<std::string>);
    
    Node *getNodeWithName(std::string);
    Command *getCommandWithName(std::string);
    std::vector<Command *> getAllCommands();

    static std::vector<Command *> BuildCommands(std::vector<std::string> cmds)
    {
        Parser parser(cmds);
        return parser.getAllCommands();
    }

private:
    Node *root;
};
