#include"./forms.h"
#include"./value.h"
#include"./error.h"
#include"./builtins.h"
#include"./tokenizer.h"
#include"./token.h"
#include"./parser.h"
 const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", defineForm}, 
    {"if", ifForm} ,
    {"and", andForm} ,
    {"or", orForm} ,
    {"quote", quoteForm}, 
     {"lambda",lambdaForm} ,
     {"cond", condForm},
     {"begin", beginForm},
     {"let", letForm},
    {"quasiquote", quasiquoteForm},
     {"`", quasiquoteForm},  
     {",", unquoteForm},
    {"unquote", unquoteForm}, 
      {"set!", setForm}, 
 };
 ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env)
 {
     std::vector<std::string> params;
     auto temp = args[0]->toVector();
     for (auto i = temp.begin(); i != temp.end(); i++)
     {
         if (auto name = (*i)->asSymbol())
         {
             params.push_back(*name);
         }
     }
     env.shared_from_this();
     std::vector<ValuePtr> body;
     int pos = 1;
     while (pos < args.size()) {
         body.push_back(args[pos]);
         pos++;
     }
     return std::make_shared<LambdaValue>(params, body,env.shared_from_this());
 }
     
 ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
     if (args.empty()) throw LispError("define error");
     
     if (auto name = args[0]->asSymbol()) {
         //在这里求值，在definebingding中只绑定
         if (args.size() != 2) throw LispError("define error");
         env.defineBinding(*name, env.eval(args[1]));
         return std::make_shared<NilValue>();
     } 
     else if(typeid(*args[0])==typeid(PairValue)) {
         //下面完成lambda
         auto& part1 = static_cast<PairValue&>(*args[0]);
         if (auto name = part1.getCar()->asSymbol()) {
             auto x = part1.getCdr()->toVector();  // x是参数列表，y是过程体
             std::vector<std::string> params;
             for (auto i = x.begin(); i != x.end(); i++)
             {

                   if (auto name = (*i)->asSymbol())
                     params.push_back(*name);
                   else
                         throw LispError("the argument in the lambda is not SymbolValue");
                         
             }
             std::vector<ValuePtr> body;
             int pos = 1;
             while (pos < args.size())
             {
                   body.push_back(args[pos]);
                   pos++;
             }
             if (body.empty()) throw LispError("no exxpression for procedure body");
             env.defineBinding(*name, std::make_shared<LambdaValue>(params, body,env.shared_from_this()));
             return std::make_shared<NilValue>();
         } else
             throw LispError("define error");
     }
     else
     {
         throw LispError("define error");
     }
 }
ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    return args[0];
}
ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    auto result = env.eval(args[0]);
    if (typeid(*result) == typeid(BooleanValue))
    {
         auto& v = static_cast<BooleanValue&>(*result);
         if (!v.getValue())
         {
             return env.eval(args[2]);
         } else {
             return env.eval(args[1]);
         }
    }
    else
         {
         return env.eval(args[1]);
         }
}
ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env)
{
         if (!args.empty()) {
         auto i = args.begin();
         for (; i != args.end(); i++) {
             auto result = env.eval(*i);
             if (typeid(*result) == typeid(BooleanValue)) {
                 auto& v = static_cast<BooleanValue&>(*result);
                 if (!v.getValue()) {
                     return std::make_shared<BooleanValue>(0);
                 }
             }
         }
         return env.eval(*(--i));
         } 
         else {
         return std::make_shared<BooleanValue>(1);
         }
}
ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env)
{
         if (!args.empty()) {
         auto i = args.begin();
         for (; i != args.end(); i++) {
             auto result = env.eval(*i);
             if (typeid(*result) != typeid(BooleanValue)) {
                 return result;
             }
             else
             {
                 auto& v = static_cast<BooleanValue&>(*result);
                 if (v.getValue()) {
                     return std::make_shared<BooleanValue>(1);
                 }
             }
         }
         return std::make_shared<BooleanValue>(0);
         } 
         else {
         return std::make_shared<BooleanValue>(0);
         }
}
ValuePtr condForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
         ValuePtr condition,result,expr;
    for (auto i = args.begin(); i != args.end(); i++)
    {
        //保证子句形式正确
        if (typeid(*(*i)) == typeid(PairValue))
        {
             auto& pair = static_cast<PairValue&>(*(*i));
             if (pair.getCar()->toString() == "else") {
                 if (i == (--args.end()))
                     condition = std::make_shared<BooleanValue>(1);
                 else
                     throw LispError("else should not appear here");
             }
              else
                 condition = env.eval(pair.getCar());
             expr = pair.getCdr();
              if (expr->toVector().size() == 1) {
                 if (typeid(*condition) == typeid(BooleanValue)) {
                     if (static_cast<BooleanValue&>(*condition).getValue() == 1)
                     {
                             result = env.eval(expr->toVector()[0]);//这个【0】可能是解析不正确导致的
                         break;
                     }  
                 } 
                 else
                     throw LispError("invalid condition in subexpression");
              }
              else
              {
                 result = condition;
              }
        } else
             throw LispError("invalid subexpression");
    }
    return result;

}
ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env) {

    ValuePtr result;
    for (auto i = args.begin(); i != args.end(); i++)
    {
        result=env.eval(*i);
    }
    return result;
}
ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    std::vector<std::string> params;
    std::vector<ValuePtr> args1;
    std::vector<ValuePtr> newdefine = args[0]->toVector();
    for (auto i = newdefine.begin(); i != newdefine.end(); i++)
    {
        if (typeid(*(*i)) == typeid(PairValue)) {
             auto& pair = static_cast<PairValue&>(*(*i));
             if (typeid(*pair.getCar()) == typeid(SymbolValue)) {

                 params.push_back(static_cast<SymbolValue&>(*pair.getCar()).getName());
                 args1.push_back(env.eval(pair.toVector()[1]));
             } 
             else throw LispError("not a variable name");
        } else
             throw LispError("not a valid expression in let form");
    }
    auto subenvi = env.createChild(params, args1);
    ValuePtr result;
    for (auto i = ++args.begin(); i != args.end(); i++)
    {
        result = subenvi->eval(*i);
    }
    return result;
}
ValuePtr quasiquoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    std::vector<ValuePtr> v;
    auto args1 = args[0]->toVector();
    auto args2 = args1[0]->toVector();
    for (auto i = args1.begin(); i != args1.end(); i++)
    {
        if (typeid(*(*i)) == typeid(PairValue))
        {
             auto name = static_cast<PairValue&>(*(*i)).getCar()->toString();
             if (name == "unquote")
             {
                 v.push_back(env.eval(
                     static_cast<PairValue&>(*(*i)).getCdr()->toVector()[0]));
             }
             else
                 v.push_back(*i);
        } else
             v.push_back(*i);
    }
    return mylist(v, env);
}
ValuePtr unquoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    throw LispError("unknown unquoteform");
}
ValuePtr setForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (auto name = args[0]->asSymbol()) {
        if (env.SymbolList.find(*name) != env.SymbolList.end()) { 
             env.defineBinding(*name, env.eval(args[1]));
             return std::make_shared<NilValue>();
        } else
             throw LispError("varible "+*name+" not defined");
    } 
    else if (typeid(*args[0]) == typeid(PairValue)) {

        if (env.SymbolList.find(*name) != env.SymbolList.end()) { 
        auto& part1 = static_cast<PairValue&>(*args[0]);
        if (auto name = part1.getCar()->asSymbol()) {
             auto x = part1.getCdr()->toVector();  // x是参数列表，y是过程体
             std::vector<std::string> params;
             for (auto i = x.begin(); i != x.end(); i++) {
                 if (auto name = (*i)->asSymbol())
                     params.push_back(*name);
                 else
                     throw LispError(
                         "the argument in the lambda is not SymbolValue");
             }
             std::vector<ValuePtr> body;
             int pos = 1;
             while (pos < args.size()) {
                 body.push_back(args[pos]);
                 pos++;
             }
             env.defineBinding(*name, std::make_shared<LambdaValue>(params, body,env.shared_from_this()));
             return std::make_shared<NilValue>();
        } else
             throw LispError("varible not defined");
        }
    } else {
        throw LispError("define error");
    }
}
