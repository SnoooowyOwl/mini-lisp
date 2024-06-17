#ifndef FORMS_H
#define FORMS_H

#include<unordered_map>
#include<string>
#include"./value.h"
#include"./eval_env.h"

using SpecialFormType= ValuePtr (const std::vector<ValuePtr>&,EvalEnv&);
extern const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS;


ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env);


ValuePtr condForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr condForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr quasiquoteForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr unquoteForm(const std::vector<ValuePtr>& args, EvalEnv& env);
////////////////////以下是补充内容
ValuePtr setForm(const std::vector<ValuePtr>& args, EvalEnv& env);//给已定义的变量赋值set!
#endif
