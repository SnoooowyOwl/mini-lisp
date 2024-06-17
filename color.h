#ifndef COLOR_H
#define COLOR_H

#include<vector>
#include<string>
#include"./token.h"

#define NONE "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define PURPLE "\033[35m"
#define GREY "\033[37m"

std::string readOneSentenceFile(std::istream& input);
extern std::vector<std::string> keywords;
std::string readOneSentence(std::istream& input, bool repl);
int getOneChar(char& r);
class colorToken {
public:
    virtual ~colorToken() = default;
};
class colorfulSTRING : public colorToken {
public:
    std::string content;
    colorfulSTRING(std::string s) : content {s}{}
};
class colorfulIDENTIFIER : public colorToken {
public:
    std::string content;
    colorfulIDENTIFIER(std::string s) : content{s} {}
};
class colorfulSPACE : public colorToken {
public:
    std::string content;
    colorfulSPACE(std::string s) : content{s} {}
};
class colorfulNOTE : public colorToken {
public:
    std::string content;
    colorfulNOTE(std::string s) : content{s} {}
};
class colorfulDOT : public colorToken {
public:
    std::string content;
    colorfulDOT(std::string s) : content{s} {}
};
class colorfulNUM : public colorToken {
public:
    std::string content;
    colorfulNUM(std::string s) : content{s} {}
};
class colorfulBOOL : public colorToken {
public:
    std::string content;
    colorfulBOOL(std::string s) : content{s} {}
};
class colorfulQUOTE : public colorToken {
public:
    std::string content;
    colorfulQUOTE(std::string s) : content{s} {}
};
class colorfulPAREN : public colorToken {
public:
    std::string content;
    colorfulPAREN(std::string s) : content{s} {}
};
using ColorPtr=std::shared_ptr<colorToken>;
std::vector<ColorPtr> preProcess(const std::string &s);
ColorPtr getToken(const std::string& s, int& pos);
void updateScr(std::vector<ColorPtr> tokens, int row);

#endif
