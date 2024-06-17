#include<deque>
#include<memory>
#include<utility>
#include"./parser.h"
#include"./value.h"
#include"./error.h"
#include<iostream>
Parser::Parser(std::deque<TokenPtr> tokens) : tokens{std::move(tokens)} {

}
ValuePtr Parser::parse() {
    if (tokens.empty()) {
        return std::make_shared<NilValue>();
       // throw SyntaxError("Unimplemented");
    }
     auto& token=tokens.front() ;
    if (token->getType() == TokenType::NUMERIC_LITERAL) {
         auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        tokens.pop_front();
        return std::make_shared<NumericValue>(value);
    }
    if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        tokens.pop_front();
        return std::make_shared<BooleanValue>(value);
    }
    if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        tokens.pop_front();
        return std::make_shared<StringValue>(value);
    }
    if (token->getType() == TokenType::IDENTIFIER) {
        auto value = static_cast<IdentifierToken&>(*token).getName();
        tokens.pop_front();
        return std::make_shared<SymbolValue>(value);
    } 
    if (token->getType() == TokenType::QUOTE)
    {
        tokens.pop_front();
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    if (token->getType() == TokenType::QUASIQUOTE)  
    {
        tokens.pop_front();
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quasiquote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    if (token->getType() == TokenType::UNQUOTE) 
    {
        tokens.pop_front();
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("unquote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    if (token->getType() == TokenType::LEFT_PAREN ||
        token->getType() == TokenType::DOT ||
        token->getType() == TokenType::RIGHT_PAREN) {//后面俩去掉
        tokens.pop_front();
       return parseTails();
    }
    
        
    
   throw SyntaxError("Unimplemented");
}
ValuePtr Parser::parseTails(){
    
    auto& token = tokens.front();
   if ((*(tokens.begin()))->getType() == TokenType::RIGHT_PAREN)
    {
        tokens.pop_front();
        return std::make_shared<NilValue>();
   }
    auto car = this->parse();
    if (tokens.empty()) 
        throw SyntaxError("Unimplemented");
    if ((*(tokens.begin()))->getType() == TokenType::DOT)  //(token1->getType() == TokenType::DOT)
    {
        tokens.pop_front();
        auto cdr = this->parse();
        tokens.pop_front();//再弹出一个词法标记，它应该是‘）’
        return std::make_shared<PairValue>(car, cdr);
    }
    else
    {
        auto cdr = this->parseTails();
        return std::make_shared<PairValue>(car, cdr);
    }
 }
