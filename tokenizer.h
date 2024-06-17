#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <deque>
#include <string>

#include "./token.h"

class Tokenizer {
private:
    TokenPtr nextToken(int& pos,bool raw,bool finaraw);
    std::deque<TokenPtr> tokenize(bool raw,bool finalraw);

    std::string input;
    Tokenizer(const std::string& input) : input{input} {}

public:
    static std::deque<TokenPtr> tokenize(std::string input,bool raw ,bool finalraw);  // const
};

#endif
