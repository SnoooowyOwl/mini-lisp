#include <iostream>
#include <string>
#include<fstream>

#include"./error.h"
#include"./value.h"
#include"./parser.h"
#include"./rjsj_test.hpp"
#include"./eval_env.h"
#include"./color.h"
#include"./token.h"
#include "./tokenizer.h"

EvalEnv env;
auto global = std::make_shared<EvalEnv>(env);


struct TestCtx {
    //EvalEnv env;
    std::shared_ptr<EvalEnv> env = std::make_shared<EvalEnv>();
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input,false,false);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env->eval(std::move(value));
        return result->toString();
    }
};
void Print(ValuePtr result) {
        if (typeid(*result)==typeid(NumericValue))std::cout <<YELLOW<< result->toString()<<NONE << std::endl;
        else if (typeid(*result)==typeid(StringValue))std::cout <<CYAN<< result->toString()<<NONE << std::endl;
        else if (typeid(*result)==typeid(BooleanValue))std::cout <<GREEN<< result->toString()<<NONE << std::endl;
        else std::cout <<NONE<< result->toString()<< std::endl;
}
void run(std::istream& input, bool repl) {
    
    while (true) {
        try {
            if(repl)std::cout <<NONE<< ">>> ";
            std::string sentence = readOneSentence(input,repl);
            auto tokens = Tokenizer::tokenize(sentence,false,false);
            Parser parser(std::move(tokens));
            auto value = parser.parse();
            
            if (typeid(*value) != typeid(NilValue)) {
                        auto result = global->eval(std::move(value));
                        if (repl) Print(result);
            }
        } catch (std::runtime_error& e) {
            std::cerr <<NONE<< "Error: " << e.what() << std::endl;
        }
    }
}
int main(char* argc, char** argv) {
    RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4,Lv5,Lv5Extra,Lv6,Lv7,Lv7Lib,Sicp);
    if (argv[1] != nullptr) {
        std::ifstream in(argv[1]);
        if (in.is_open()) {
            run(in, 0);
        } else {
            std::cout << "cannot open " << argv[1] << std::endl;
        }
    } else {
        run(std::cin, 1);
    }
}
