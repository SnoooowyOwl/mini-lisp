#include <iostream>
#include <string>
#include<fstream>
#include"./error.h"
#include "./tokenizer.h"
#include"./value.h"
#include"./parser.h"
#include"./rjsj_test.hpp"
#include"./eval_env.h"
struct TestCtx {
    //EvalEnv env;
    std::shared_ptr<EvalEnv> env = std::make_shared<EvalEnv>();
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env->eval(std::move(value));
        return result->toString();
    }
};
std::string readOneSentence(std::istream& input) {

    std::string sentence;
   
    char c = input.get();
    if (c == EOF) std::exit(0);

    while (c == ' ' || c == '\n' || c == '\t') {//读到第一个可显示字符，要么是(,要么不是，否则错误
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
            else if (r == ';')
            {
                std::string notes;
                std::getline(input, notes);//notes应该是注释的内容
                continue;
            }
            std::string s1{r};
            sentence += s1;
        }
        c = input.get();  // 应该读到一个'\n'
        if (c == ';')
        {
            std::string notes;
            std::getline(input, notes);  // notes应该是注释的内容
        }
    } 
    else {
        std::string s1{c}, s2;
        std::getline(input, s2);
        sentence = s1 + s2;
    }
    return sentence;
}
void run(std::istream& input,bool repl) {
    EvalEnv env;
    auto global = std::make_shared<EvalEnv>(env);
    while (true) {
        try {
            if(repl)std::cout << ">>> ";
            std::string sentence = readOneSentence(input);
            auto tokens = Tokenizer::tokenize(sentence);
            Parser parser(std::move(tokens));
            auto value = parser.parse();
            auto result = global->eval(std::move(value));
            if(repl)std::cout << result->toString() << std::endl;
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
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
