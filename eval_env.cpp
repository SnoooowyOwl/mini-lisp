#include"./eval_env.h"
#include"./error.h"
#include"./builtins.h"
#include"./token.h"
#include"./forms.h"
#include<algorithm>
#include<iterator>
#include<iostream>
#include<unordered_map>



EvalEnv::EvalEnv(std::shared_ptr<EvalEnv> p,
                 std::unordered_map<std::string, ValuePtr> S)
    : parent{p}, SymbolList{S} {
 }
void EvalEnv::defineBinding(std::string name, ValuePtr args)
{///////////////////////////////////////这里的原始版本是this->eval(args)，为了解决lambda表达式中不可以对列表求值的问题
    SymbolList.insert_or_assign(name,args );
    return;
}
EvalEnv::EvalEnv() {
    for (auto i : BuiltinFuncsList) {
        SymbolList.insert(i);
    }
}
ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isNil())
        throw LispError("Evaluating nil is prohibited.");
    else if (expr->isSelfEvaluating()){
        return expr;
    }
    else if (typeid(*expr)==typeid(SymbolValue)) {

            auto name = expr->asSymbol();
            auto value = SymbolList.find(*name);
            if (value != SymbolList.end()) {
                  return  value->second;
            } else {
                throw LispError("Variable " + *name + " not defined.");
            }
    }
    else if (typeid(*expr) == typeid(PairValue)) {
            auto& expression = static_cast<const PairValue&>(*expr);
            using namespace std::literals;
            std::vector<ValuePtr> v = expr->toVector();
            if (auto name = v[0]->asSymbol()) {
                if (SPECIAL_FORMS.find(*name)!= SPECIAL_FORMS.end()) {

                    auto v1 = expression.getCdr()->toVector();  // 用来检查
                    return SPECIAL_FORMS.at(*name)(v1, *this);//此处有问题
                } 
                else {
                    ValuePtr proc = this->eval(v[0]);
                    auto& pair = static_cast<const PairValue&>(*expr);
                    std::vector<ValuePtr> args = evalList(pair.getCdr());
                    return this->apply(proc, args);
                }
            } 
            else {
                ValuePtr proc = this->eval(v[0]);
                auto& pair = static_cast<const PairValue&>(*expr);
                std::vector<ValuePtr> args = evalList(pair.getCdr());
                return this->apply(proc, args);
            }
    } 
    else if (typeid(*expr) == typeid(LambdaValue))
    {
            return expr;
    }
    else
    {
            throw LispError("UnKnown Value");
    }
}
std::vector<ValuePtr>EvalEnv::evalList(ValuePtr expr)
{
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVector(),std::back_inserter(result),[this](ValuePtr v){
        return this->eval(v);});
    return result;
}
ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr>args)
{
    if (typeid(*proc) == typeid(BuiltinProValue)) {
        auto& f=static_cast<const BuiltinProValue&>(*proc);
        return f.call()(args,*this);
    } 
    else if (typeid(*proc) == typeid(LambdaValue))
    {
        auto& lambda = static_cast<LambdaValue&>(*proc);
        //auto inner = createChild()
        return lambda.apply(args);
    }
    else {
    throw LispError("Unknown procedure");
    }
}
ValuePtr EvalEnv::lookupBinding(std::string name)
{
    
        auto value = SymbolList.find(name);
            if (value != SymbolList.end()) {
                return value->second;
            } 
            else if(this->parent!=nullptr){
                return this->parent->lookupBinding(name);
             } 
            else
            {
             throw LispError("Variable " + name + " not defined.");
            }
}
void EvalEnv::setParent(std::shared_ptr<EvalEnv> p)
{
            parent = p;
}
std::shared_ptr<EvalEnv> EvalEnv::getParent() {
            return parent;
}
std::shared_ptr<EvalEnv> EvalEnv::createChild(const std::vector<std::string>& params, const std::vector<ValuePtr>& args)
{
            EvalEnv inner{*this};
            inner.setParent(make_shared<EvalEnv>(*this));
            auto i = params.begin();
            auto j = args.begin();
            if (params.size() != args.size())
             throw LispError("mismatched arguments");
            for (; i != params.end() && j != args.end(); i++,j++)
            {
             inner.defineBinding(*i, *j);
            }
            return make_shared<EvalEnv>(inner);
}
