#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stack>

class Command;
class DoubleCmd;

enum TokenType {
    CMD = 0,
    ARG = 1,
    PIPE = 2,
    LEFT = 3,
    RIGHT = 4,
    BACKGROUND=5,
    TWO=6,
    DELIM=7
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
    std::vector <Node *> nodes;
    bool _buildAST(std::vector<Token *>::iterator pos, std::vector<Token *>::iterator end, Node *parent);
    std::vector <Token *> tokenizer;
    bool _parseCMD(Node *node, std::vector<Token *>::iterator &iter, std::vector<Token *> &tokens);
    void _dfs(Node *node, std::vector<Command *> &coms, std::vector<Node *> &discovered, Command * cmd, Command *curCMD, std::stack <DoubleCmd *> &doubleCmds);
    bool _parseArgs(Node *node, std::vector<Token *>::iterator &iter, std::vector<Token *> &tokens);
    Node *root = new Node;
};
