#ifndef BUILTINS_H
#define BUILTINS_H
#include"./value.h"
#include<unordered_map>
bool judgelist(ValuePtr p);
bool isequal(ValuePtr x, ValuePtr y);

extern std::unordered_map<std::string, std::shared_ptr<BuiltinProValue>> BuiltinFuncsList;
    
ValuePtr Apply(const std::vector<ValuePtr>& params, EvalEnv& env);  //apply
ValuePtr display(const std::vector<ValuePtr>& params, EvalEnv& env);    //display
ValuePtr displayLN(const std::vector<ValuePtr>& params, EvalEnv& env);      //displayln
ValuePtr error(const std::vector<ValuePtr>& params, EvalEnv& env);  //+error
ValuePtr myeval(const std::vector<ValuePtr>& params, EvalEnv& env);     //eval
ValuePtr Exit(const std::vector<ValuePtr>& params, EvalEnv& env);    //exit
ValuePtr newline(const std::vector<ValuePtr>& params, EvalEnv& env);     //newline

ValuePtr add(const std::vector<ValuePtr>& params, EvalEnv& env);//+
ValuePtr subtract(const std::vector<ValuePtr>& params, EvalEnv& env);//-
ValuePtr mul(const std::vector<ValuePtr>& params, EvalEnv& env);//*
ValuePtr divide(const std::vector<ValuePtr>& params, EvalEnv& env);  //除
ValuePtr Abs(const std::vector<ValuePtr>& params, EvalEnv& env);//abs
ValuePtr expt(const std::vector<ValuePtr>& params, EvalEnv& env);//expt
ValuePtr quotient(const std::vector<ValuePtr>& params, EvalEnv& env);//quotient
ValuePtr modulo(const std::vector<ValuePtr>& params, EvalEnv& env);//modulo
ValuePtr Remainder(const std::vector<ValuePtr>& params, EvalEnv& env);//reminder

ValuePtr eq(const std::vector<ValuePtr>& params, EvalEnv& env);//eq?
ValuePtr equal(const std::vector<ValuePtr>& params, EvalEnv& env);//equal
ValuePtr Equal(const std::vector<ValuePtr>& params, EvalEnv& env);//=
ValuePtr Not(const std::vector<ValuePtr>& params, EvalEnv& env); // not

ValuePtr isSmaller(const std::vector<ValuePtr>& params, EvalEnv& env);//<
ValuePtr isBigger(const std::vector<ValuePtr>& params, EvalEnv& env);//>
ValuePtr isEqualSmaller(const std::vector<ValuePtr>& params, EvalEnv& env);//<=
ValuePtr isEqualBigger(const std::vector<ValuePtr>& params, EvalEnv& env);//>=
ValuePtr even(const std::vector<ValuePtr>& params, EvalEnv& env);//even
ValuePtr odd(const std::vector<ValuePtr>& params, EvalEnv& env);//odd
ValuePtr zero(const std::vector<ValuePtr>& params, EvalEnv& env);//zero

ValuePtr atom(const std::vector<ValuePtr>& params, EvalEnv& env); // atom
ValuePtr integer(const std::vector<ValuePtr>& params, EvalEnv& env);//integer
ValuePtr islist(const std::vector<ValuePtr>& params, EvalEnv& env);//list?
ValuePtr boolean(const std::vector<ValuePtr>& params, EvalEnv& env);//boolean
ValuePtr number(const std::vector<ValuePtr>& params, EvalEnv& env);//number
ValuePtr null(const std::vector<ValuePtr>& params, EvalEnv& env);//null
ValuePtr pair(const std::vector<ValuePtr>& params, EvalEnv& env);//pair
ValuePtr procedure(const std::vector<ValuePtr>& params, EvalEnv& env);//procedure
ValuePtr string(const std::vector<ValuePtr>& params, EvalEnv& env);//string
ValuePtr symbol(const std::vector<ValuePtr>& params, EvalEnv& env);//symbol

ValuePtr append(const std::vector<ValuePtr>& params, EvalEnv& env);//append
ValuePtr car(const std::vector<ValuePtr>& params, EvalEnv& env);//car
ValuePtr cdr(const std::vector<ValuePtr>& params, EvalEnv& env);//cdr
ValuePtr cons(const std::vector<ValuePtr>& params, EvalEnv& env);//cons
ValuePtr length(const std::vector<ValuePtr>& params, EvalEnv& env);//length
ValuePtr mylist(const std::vector<ValuePtr>& params, EvalEnv& env);//list!!!!!!!!
ValuePtr map(const std::vector<ValuePtr>& params, EvalEnv& env);//map
ValuePtr filter(const std::vector<ValuePtr>& params, EvalEnv& env);//fliter
ValuePtr reduce(const std::vector<ValuePtr>& params, EvalEnv& env);//reduce

ValuePtr print(const std::vector<ValuePtr>& params, EvalEnv& env);//print
/////////////////////////以下是添加内容/////////////////////////////
ValuePtr WHen(const std::vector<ValuePtr>& params, EvalEnv& env); //when
ValuePtr UNless(const std::vector<ValuePtr>& params, EvalEnv& env);  // unless
//添加若干字符串运算函数，有理数运算函数//
ValuePtr Strcat(const std::vector<ValuePtr>& params, EvalEnv& env);//字符串拼接
ValuePtr Strcmp(const std::vector<ValuePtr>& params, EvalEnv& env);//字符串比较
ValuePtr Strlen(const std::vector<ValuePtr>& params, EvalEnv& env);//字符串长度
ValuePtr Strstr(const std::vector<ValuePtr>& params, EvalEnv& env);//字符串查找
ValuePtr At(const std::vector<ValuePtr>& params, EvalEnv& env);//下标访问
ValuePtr Set(const std::vector<ValuePtr>& params, EvalEnv& env);  // 给已定义的变量赋值
#endif
