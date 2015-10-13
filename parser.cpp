/**
 * \author Yoseph Radding
 * \file parser.cpp
 *
 * Defines the parser class
 */

#include "parser.h"

#include <queue>
#include <algorithm>

#include "background.h"
#include "DoubleCmd.h"
#include "command.h"
#include "pipe.h"
#include "file.h"

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
    TokenType currentType = CMD;
    Token *lastCMD;
    
    //Tokenize input string
    for (auto cmd : commands){
        if (cmd == "" || cmd == " ") continue;
        Token *curToken = new Token;
        curToken->type = currentType;
        if(currentType == CMD){
            lastCMD = curToken;
            currentType = ARG;
        }
        if(cmd == "&"){
            lastCMD->type = BACKGROUND;
            continue;
        }
        if(cmd.find(";") != string::npos){
            currentType = CMD;
            size_t loc = cmd.find(';');
            cmd.erase(loc);
            Token *tok = new Token;
            tok->type = DELIM;
            curToken->value = cmd;
            tokenizer.push_back(curToken);
            tokenizer.push_back(tok);
            if (cmd==" " || cmd == ""){
                delete curToken;
            }
            continue;
        }
        if(cmd.find("&&") != string::npos){
            curToken->type = TWO;
            currentType = CMD;
            tokenizer.push_back(curToken);
            continue;
        }
        if(cmd.find(">") != string::npos){
            curToken->type = RIGHT;
            currentType = CMD;
            tokenizer.push_back(curToken);
            continue;
        }
        if(cmd.find("|") != string::npos){
            curToken->type = PIPE;
            currentType = CMD;
            tokenizer.push_back(curToken);
            continue;
        }
        curToken->value = cmd;
        tokenizer.push_back(curToken);
    }

    //Convert to some sort of Syntax Tree 
    _buildAST(tokenizer.begin(), tokenizer.end(), root);
}

bool Parser::_buildAST(vector<Token *>::iterator pos, vector<Token *>::iterator end, Node *parent){
    Node *node = nullptr;
    if(pos == end) return true;
    auto token = (*pos);
    switch(token->type){
        case CMD:{
            node = new Node;
            node->token = *token;
            node->parent = parent;
            parent->children.push_back(node);
            if(_buildAST(++pos, end, node)) return true;
            else return _buildAST(++pos, end, parent);

            break;
        }
        case BACKGROUND: {
            node = new Node;
            node->token = *token;
            parent->children.push_back(node);
            node->parent = parent;

            if(_buildAST(++pos, end, node)) return true;
            else return _buildAST(++pos, end, parent);

            break;
        }
        case ARG: {
            node = new Node;
            node->token = *token;
            node->parent = parent;
            parent->children.push_back(node);
            return _buildAST(++pos, end, parent);
            break;
        }
        case DELIM: {
            return false;
            break;
        }
        case PIPE: {
            node = new Node;
            node->token = *token;
            Node *par = parent->parent;
            Node *lastCMD = parent;
            par->children.pop_back();
            par->children.push_back(node);
            node->children.push_back(lastCMD);
            node->parent = par;
            lastCMD->parent = node;
            if(_buildAST(++pos, end, node)) return true;
            else{
                return _buildAST(++pos, end, parent);
            }

            break;
        }
        case TWO: {
            node = new Node;
            node->token = *token;
            Node *par = parent->parent;
            Node *lastCMD = parent;
            par->children.pop_back();
            par->children.push_back(node);
            node->children.push_back(lastCMD);
            node->parent = par;
            lastCMD->parent = node;
            if(_buildAST(++pos, end, node)) return true;
            else{
                return _buildAST(++pos, end, parent);
            }

            break;
        }
        case RIGHT: {
            node = new Node;
            node->token = *token;
            Node *par = parent->parent;
            Node *lastCMD = parent;
            par->children.pop_back();
            par->children.push_back(node);
            node->children.push_back(lastCMD);
            node->parent = par;
            lastCMD->parent = node;
            _buildAST(pos+1, pos+2, node); 
            break;
            
        }
    }
    return true;
}

bool Parser::_parseArgs(Parser::Node *node, vector<Token *>::iterator &iter, vector<Token *> &tokens){
    for(;iter != tokens.end() && (*iter)->type == ARG; iter++) {
        Node *argNode = new Node;
        argNode->token = *(*iter);
    }
    return true;
}

bool Parser::_parseCMD(Parser::Node *node, vector<Token *>::iterator &iter, vector<Token *> &tokens){
    
    for(; (*iter)->type == ARG && iter != tokens.end(); iter ++){
        Node *cmdArg = new Node;
        cmdArg->token = *(*iter);
        node->children.push_back(cmdArg);
    }
    if(iter == tokens.end()) return false;
    if((*iter)->type == TWO || (*iter)->type == PIPE){
        Node *secondTwo = new Node;
        secondTwo->token = *(*iter);
        iter ++;
        if((*iter)->type == CMD)
        {
            Node *nextCMD = new Node;
            nextCMD->token = *(*iter);
            _parseCMD(nextCMD, iter, tokens);
            node->children.push_back(nextCMD);
        }
    }

    return true;
}

void Parser::_dfs(Node *node, vector<Command *> &coms, vector<Node *> &discovered, Command * cmd, Command *curCMD, stack <DoubleCmd *> &doubleCmds){
    discovered.push_back(node);
    
    switch (node->token.type){
        case CMD: {
            cmd = new Command();
            if(!doubleCmds.empty()){
                DoubleCmd *bCMD = doubleCmds.top();
                if(!bCMD->cmdsFilled())
                    bCMD->addCmd(cmd);
                if(bCMD->cmdsFilled())
                    doubleCmds.pop();

            }
            else {
                curCMD = cmd;
                coms.push_back(cmd);
            }
            cmd->setName(node->token.value);
            break;
        }
        case ARG:  {
            cmd->addArg(node->token.value);
            break;
        }
        case BACKGROUND: {
            cmd = new BackgroundCommand();
            cmd->setName(node->token.value);
            coms.push_back(cmd);
            break;
        }
        case TWO: {
            cmd = new DoubleCmd();
            if (!doubleCmds.empty()){
                DoubleCmd *oldOne = doubleCmds.top();
                oldOne->addCmd(cmd);
                if(oldOne->cmdsFilled())
                    doubleCmds.pop();
            }
            doubleCmds.push(dynamic_cast<DoubleCmd *>(cmd));
            cmd->setType("double");
            coms.push_back(cmd);
            break;
        }
        case PIPE: {
            cmd = new Pipe();
            if (!doubleCmds.empty()){
                DoubleCmd *oldOne = doubleCmds.top();
                oldOne->addCmd(cmd);
                if(oldOne->cmdsFilled())
                    doubleCmds.pop();
            }
            doubleCmds.push(dynamic_cast<DoubleCmd *>(cmd));
            cmd->setType("double");
            coms.push_back(cmd);
            break;
        }
        case RIGHT: {
           cmd = new Right(node->children.back()->token.value);
           coms.push_back(cmd);
            cmd->setName(node->token.value);
           break;
        }
    }

    for(auto i : node->children){
        if(find(discovered.begin(), discovered.end(), i) == discovered.end())
            _dfs(i, coms, discovered, cmd, curCMD, doubleCmds);
    }
}
/** 
 * takes the nodes from the parser and builds commands from that by doing a depth first search
 */
std::vector<Command *> Parser::getAllCommands()
{
    vector<Command *> commands;
    vector<Node *> discovered;
    stack <DoubleCmd *> doubleCmds;
    for(auto i : root->children){
        _dfs(i, commands, discovered, nullptr, nullptr, doubleCmds);       
    }
    return commands;
}

Parser::~Parser()
{
    queue<Node *> nodes;
    vector<Node *> discovered;
    //nodes.push(root);
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
