#include"./color.h"
#include<Windows.h>
#include"./error.h"
#include<conio.h>
#include<iostream>
#include"./eval_env.h"

extern EvalEnv env;
extern std::vector<std::string> keywords = {
    "define", "if",    "and", "or",         "lambda",
    "cond",   "begin", "let", "quasiquote", "unquote"};
void updateScr(std::vector<ColorPtr> tokens, int row) {
    HANDLE hStdout;
    CONSOLE_SCREEN_BUFFER_INFO pBuffer;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(hStdout, &info);
    info.bVisible = 0;
    SetConsoleCursorInfo(hStdout, &info);
    GetConsoleScreenBufferInfo(hStdout, &pBuffer);
    int cursorx = pBuffer.dwCursorPosition.X;
    int cursory = pBuffer.dwCursorPosition.Y;

    COORD cd{0, cursory + 1 - row};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
    for (int i = 1; i <= row; i++) {
        std::cout << '\r';
        std::cout << "                                                                                                                                                          ";
        std::cout << '\r';
        if (i != row) std::cout << '\n';
    }
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
    std::cout << NONE << ">>> ";
    for (auto i = tokens.begin(); i != tokens.end(); i++)
    {
        if (typeid(*(*i)) == typeid(colorfulPAREN))
        {
            std::cout << RED << static_cast<colorfulPAREN&>(*(*i)).content << NONE;
        }
        else if (typeid(*(*i)) == typeid(colorfulDOT))
        {
            std::cout << RED << static_cast<colorfulDOT&>(*(*i)).content << NONE;
        }
        else if (typeid(*(*i)) == typeid(colorfulQUOTE))
        {
            std::cout << RED << static_cast<colorfulQUOTE&>(*(*i)).content << NONE;
        }
        else if (typeid(*(*i)) == typeid(colorfulNUM)) {
            std::cout << YELLOW << static_cast<colorfulNUM&>(*(*i)).content << NONE;
        }
        else if (typeid(*(*i)) == typeid(colorfulBOOL)) {
            std::cout <<GREEN<< static_cast<colorfulBOOL&>(*(*i)) .content << NONE;
        }
        else if (typeid(*(*i)) == typeid(colorfulSTRING)) {
            std::cout << CYAN << static_cast<colorfulSTRING&>(*(*i)).content << NONE;
        }
        else if (typeid(*(*i)) == typeid(colorfulIDENTIFIER)) {
            if (std::find(keywords.begin(), keywords.end(), static_cast<colorfulIDENTIFIER&>(*(*i)).content) != keywords.end())
            {
                std::cout <<BLUE<< static_cast<colorfulIDENTIFIER&>(*(*i)).content << NONE;
            }
            
            else if (env.SymbolList.find(static_cast<colorfulIDENTIFIER&>(*(*i)).content)!=env.SymbolList.end())
            {
                std::cout <<PURPLE<< static_cast<colorfulIDENTIFIER&>(*(*i)).content << NONE;
            } 
            else {
                std::cout << GREY<<static_cast<colorfulIDENTIFIER&>(*(*i)).content << NONE;
            }
        }
        else if (typeid(*(*i)) == typeid(colorfulNOTE)) {
            std::cout << YELLOW << static_cast<colorfulNOTE&>(*(*i)).content << NONE;
        }
        else if (typeid(*(*i)) == typeid(colorfulSPACE)) {
            /*
            for (int j = 0; j < (*i)->content.length(); j++)
            {
                std::cout << (*i)->content[j];
            }
            */
            std::cout << static_cast<colorfulSPACE&>(*(*i)).content;
        }
        
    }
    info.bVisible = 1;
    SetConsoleCursorInfo(hStdout, &info);
}

int getOneChar(char& target) {
    char tmp = _getch();  // 读取键值，或过滤功能键的第一个返回值
    while (true) {
        if (tmp == 0 ||tmp ==-32)  // 表示读取的是功能键或者方向键，丢掉第一个返回值，读取第二个返回值
        {
            target = _getch();
            return 0;
        } else  // 普通按键，如字母、数字、space，Esc等按键
        {
            target = tmp;
            return tmp;
        }
    }
}
std::string readOneSentence(std::istream& input, bool repl) {
    std::string sentenceBuffer;
    bool parenexpr=0;
    char c, r;
    int row=1;
    int count = 0;
    if (repl) {
        do {
            int validChar = getOneChar(r);
            if (validChar)  // 读取到了普通按键
            {
                // 退格和回车特殊处理一下
                if (r < 0) throw LispError("not avaliable in Chinese");
                if (r == '\r')  // 实际上是按下了enter
                {
                    sentenceBuffer += '\n';
                    c = r = '\n';
                    std::cout << '\n';
                    row++;
                } else if (r == '\b')  // 实际上是按下了BKSP
                {
                    sentenceBuffer =
                        sentenceBuffer.substr(0, sentenceBuffer.length() - 1);

                } else {
                    c = r;
                    sentenceBuffer += std::string{c};
                }
            } else  // 读取到了功能键，维护字符串
            {
                throw LispError("undefined button");  // 还不能实现光标定位
            }

            if (c == '(' || c == ')') {
                parenexpr = 1;
            } else if (c == '\x1a')
                std::exit(0);
            else if (!parenexpr)  // 如果c不是括号引起的，那么读完一行
            {
                sentenceBuffer = "";
                while (c != '\n' && c != '\r') {
                    if (c == '\x1a')
                        std::exit(0);
                    else if (c == '\b')
                        sentenceBuffer = sentenceBuffer.substr(
                            0, sentenceBuffer.length() - 1);
                    else
                        sentenceBuffer += std::string{c};
                    auto tokens = preProcess(sentenceBuffer);
                    updateScr(tokens, 1);
                    int validChar = getOneChar(c);
                    if (!validChar) throw LispError("undefined button");
                }
                sentenceBuffer += '\n';
                auto tokens = preProcess(sentenceBuffer);
                updateScr(tokens, 1);
                return sentenceBuffer;
            }
            auto tokens = preProcess(sentenceBuffer);
            updateScr(tokens, row);
            count = 0;
            for (auto j : tokens) {
                if (typeid(*j) == typeid(colorfulPAREN)) {
                    if (static_cast<colorfulPAREN&>(*j).content == "(")
                        count++;
                    else if (static_cast<colorfulPAREN&>(*j).content == ")")
                        count--;
                }
            }
            if (count < 0) throw LispError("mismatched paren");
        } while (!(count == 0 && c == '\n'));
        auto tokens = preProcess(sentenceBuffer);
        updateScr(tokens, row);
        return sentenceBuffer;
    }
    else
    {
        return readOneSentenceFile(input);
    }
}
ColorPtr getToken(const std::string& s, int& pos)
{
    while (pos < s.size()) {
        auto c = s[pos];
        if (c == ';') {
            std::string sentence;
            while (pos < s.size() && s[pos] != '\n') {
                sentence += std::string{s[pos]};
                pos++;
            }
            return make_shared<colorfulNOTE>(sentence);
        }
        else if (std::isspace(c)) {
            std::string sentence;
            while (pos < s.size() && std::isspace(s[pos])) {
                sentence += std::string{s[pos]};
                pos++;
            }
            return make_shared<colorfulSPACE>(sentence);
        } 
        else if (c == '(' || c ==')') {
            pos++;
            return make_shared<colorfulPAREN>(std::string{c});
        } 
        else if (c == ',' || c == '`' || c =='\'') {
            pos++;
            return make_shared<colorfulPAREN>(std::string{c});
        } 
        else if (c =='.') {
            pos++;
            return make_shared<colorfulPAREN>(std::string{c});
        } 
        else if (c == '#') {
            std::string sentence;
            while (pos < s.size() && !std::isspace(s[pos]) && s[pos] != '(' &&s[pos] !=')') {
                sentence += std::string{s[pos]};
                pos++;
            }
            return make_shared<colorfulBOOL>(sentence);
        } 
        else if (c == '"') {
            std::string sentence{c};
            pos++;
            int end = pos+1;
            while (end<s.size()) {
                end = s.find('\"', end);
                if (end == -1) {
                    end = s.size();
                    break;
                }
                if (s[end - 1] == '\\') {
                    end++;
                } 
                else
                    break;
            }
            if (end >= s.size()||end<=pos-1)
            {
                sentence += s.substr(pos, s.size() - pos);
                pos = s.size();
            }
            else
            {
                sentence += s.substr(pos, end - pos + 1);
                pos = end + 1;
            }
            /*
            while (pos < s.size()) {
                if (s[pos] == '\"' && pos+1<s.size()&&(std::isspace(s[pos + 1]) ||s[pos + 1] == '(' || s[pos + 1] == ')'))
                    break;
                sentence += std::string{s[pos]};
                pos++;
            }
            if (s[pos] == '"') {
                pos++;
                sentence += "\"";
            }
            */
            return make_shared<colorfulSTRING>(sentence);
        } 
        else if (c >= '0' && c <= '9')
        {
            pos++;
            return make_shared<colorfulNUM>(std::string{c});
        }
        else {
            std::string sentence;
            while (pos < s.size() && !std::isspace(s[pos]) && s[pos] != '(' &&s[pos] !=')') {
                sentence += std::string{s[pos]};
                pos++;
            }
            return make_shared<colorfulIDENTIFIER>(sentence);
        }
    }
    return nullptr;
}
std::vector<ColorPtr> preProcess(const std::string& s)
{
    std::vector<ColorPtr> tokens;
    int pos = 0;
    while (true) {
        auto token = getToken(s,pos);
        if (!token) {
            break;
        }
        tokens.push_back(token);
    }
    return tokens;
}
std::string readOneSentenceFile(std::istream& input) {
    std::string sentence;
    char c = input.get();
    if (c == EOF) std::exit(0);
    while (c == ' ' || c == '\n' ||
           c == '\t') {  // 读到第一个可显示字符，要么是(,要么不是，否则错误
        c = input.get();
    }
    if (c == '(') {
        int count = 1;
        sentence = "(";
        while (count != 0) {
            char r = input.get();
            if (r == '(')
                count++;
            else if (r == ')')
                count--;
            else if (r == '\n' && r == '\t')  // 其他不可见字符
                continue;
            else if (r == ';') {
                std::string notes;
                std::getline(input, notes);  // notes应该是注释的内容
                continue;
            }
            std::string s1{r};
            sentence += s1;
        }
        c = input.get();  // 应该读到一个'\n'
        if (c == ';') {
            std::string notes;
            std::getline(input, notes);  // notes应该是注释的内容
        }
    } else {
        std::string s1{c}, s2;
        std::getline(input, s2);
        sentence = s1 + s2;
    }
    return sentence;
}
