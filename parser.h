#ifndef PARSER_H 
#define PARSER_H
#include"./value.h"
#include"./token.h"
#include<deque>
class Parser
{
    std::deque<TokenPtr> tokens;

public:
    Parser(std::deque < TokenPtr >tokens);
    ValuePtr parse();
    ValuePtr parseTails();
};
#endif
