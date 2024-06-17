#include "./tokenizer.h"
#include<iostream>
#include <cctype>
#include <set>
#include <stdexcept>

#include "./error.h"

const std::set<char> TOKEN_END{'(', ')', '\'', '`', ',', '"'};

TokenPtr Tokenizer::nextToken(int& pos,bool raw,bool finalraw) {
    while (pos < input.size()) {
        auto c = input[pos];
        if (c == ';') {
            std::string sentence;
            while (pos < input.size() && input[pos] != '\n') {
                sentence += std::string{input[pos]};
                pos++;
            }
        } 
        else if (std::isspace(c)) {
            while (pos < input.size() && std::isspace(input[pos]))
            {
                pos++;
            }
        } 
        else if (auto token = Token::fromChar(c)) {
            pos++;
            return token;
        } 
        else if (c == '#') {
          if (auto result = BooleanLiteralToken::fromChar(input[pos + 1])) {
                pos += 2;
                return result;
           } 
            else {
                throw SyntaxError("Unexpected character after #");
            }
        } 
        else if (c == '"') {
            std::string string;
            pos++;
            while (pos < input.size()) {
                if (input[pos] == '"') {
                    pos++;
                    return std::make_unique<StringLiteralToken>(string);
                } else if (input[pos] == '\\') {
                    if (pos + 1 >= input.size()) {
                        throw SyntaxError("Unexpected end of string literal");
                    }
                    auto next = input[pos + 1];
                    if (next == 'n') {//racket里面是+="\\n"
                        string += '\n';
                    } 
                    else if (next == 't') {//+="\\t"
                        string += '\t';
                    }
                    else {
                        string += next;
                    }
                    pos += 2;
                } else {
                    string += input[pos];
                    pos++;
                }
            }
            throw SyntaxError("Unexpected end of string literal");
        } 
        else {
            int start = pos;
            do {
                pos++;
            } while (pos < input.size() && !std::isspace(input[pos]) &&!TOKEN_END.contains(input[pos]));
            auto text = input.substr(start, pos - start);
            if (text == ".") {
                return Token::dot();
            }
            if (std::isdigit(text[0]) || text[0] == '+' || text[0] == '-' || text[0] == '.') {
                try {
                    return std::make_unique<NumericLiteralToken>(std::stod(text));
                } catch (std::invalid_argument& e) {
                }
            }
            return std::make_unique<IdentifierToken>(text);
        }
    }
    return nullptr;
}

std::deque<TokenPtr> Tokenizer::tokenize(bool raw,bool finalraw) {//是否解析的是原始字符串，也即是解析出来的含不含空白字符
    std::deque<TokenPtr> tokens;
    int pos = 0;
    while (true) {
        auto token = nextToken(pos,raw,finalraw);
        if (!token) {
            break;
        }
        tokens.push_back(std::move(token));
    }
    return tokens;
}

std::deque<TokenPtr> Tokenizer::tokenize(std::string input,bool raw,bool finalraw) {//const
    return Tokenizer(input).tokenize(raw,finalraw);
}
