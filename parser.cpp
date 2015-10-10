/**
 * \author Yoseph Radding
 * \file parser.cpp
 *
 * Defines the parser class
 */

#include "parser.h"

#include <queue>
#include <stack>
#include <algorithm>

#include "background.h"
#include "DoubleCmd.h"
#include "command.h"

using namespace std;
/**
 * Constructor
 *
 * \param count count of commands in the command array
 * \param commands command array of c-style strings
 * 
 * Tokenizes the commands and build a simple parser tree for the commands
 */
Parser::Parser(vector<string> commands)
{   
    root = nullptr;
    TokenType currentType = CMD;
    Node *parent = nullptr;

    for (auto cmd : commands){
        if (cmd == "" || cmd == " ") continue;
        Node *node = new Node;
        node->token.type = currentType;
        if(nullptr == root){
            root = node;
            parent = root;
            currentType = ARG;
        }
        if(currentType == CMD)
            currentType = ARG;
        if(cmd == "&"){
            parent->token.type = BACKGROUND;
            delete node;
            continue;
        }
        if(cmd.find(";") != string::npos){
            currentType = CMD;
            cmd.erase(cmd.find(";"));
            if (cmd == "" || cmd == " "){
                delete node;
                continue;
            }
            else{
                parent->children.push_back(node);
                node->parent = parent;
            }
            parent = root;
        }
        if(cmd.find("&&") != string::npos){
            node->token.type = TWO;
            Node *lastNode = parent->children.back();
            parent->children.pop_back();
            node->children.push_back(lastNode);
            parent->children.push_back(node);
            parent = node;
            currentType = CMD;
            continue;
        }
        else{
            parent->children.push_back(node);
            node->parent = parent;
        }
        node->token.value = cmd;
    }
}

/** 
 * takes the nodes from the parser and builds commands from that by doing a depth first search
 */
std::vector<Command *> Parser::getAllCommands()
{
    queue <Node *> toVisit;
    stack <DoubleCmd *> doubleCMDs;
    vector <Node *> visited;
    vector <Command *> commands;
    toVisit.push(root);
    Command *cmd = nullptr;
    while(!toVisit.empty()){
        Node *node = toVisit.front();
        toVisit.pop();
        TokenType nodeType = node->token.type;
        switch(nodeType){
            case(CMD): {
                cmd = new Command();
                cmd->setName(node->token.value);
                if(!doubleCMDs.empty()) {
                    doubleCMDs.top()->addCmd(cmd);
                    if(doubleCMDs.top()->cmdsFilled())
                        doubleCMDs.pop();
                }
                else {
                    commands.push_back(cmd);
                  }
                break;
            }
            case(BACKGROUND):
                cmd = new BackgroundCommand();
                cmd->setName(node->token.value);
                commands.push_back(cmd);
                break;
            case(ARG):
                cmd->addArg(node->token.value);
                break;
            case(TWO): {
                DoubleCmd *dCmd= new DoubleCmd();
                doubleCMDs.push(dCmd);
                Command *prevCmd = commands.back();
                dCmd->addCmd(prevCmd);
                commands.pop_back();
                commands.push_back(dCmd);
                break;
            }
            default:
                break;
        }
    visited.push_back(node);
    for (auto i : node->children){
        if(find(visited.begin(), visited.end(), i) == visited.end())
            {
                toVisit.push(i);   
            }
        }
    }
    return commands;
}

Parser::~Parser()
{
    queue<Node *> nodes;
    vector<Node *> discovered;
    nodes.push(root);
    while (!nodes.empty())
    {
        Node *node = nodes.front();
        nodes.pop();
        discovered.push_back(node);
        for (auto i : node->children)
        {
            if(find(discovered.begin(), discovered.end(), i) == discovered.end())
            {
                nodes.push(i);
            }
        }
        delete node;
    }
}
