#ifndef EVAL_ENV_H
#define EVAL_ENV_H
#include"./value.h"
#include<unordered_map>
class EvalEnv : public std::enable_shared_from_this<EvalEnv>{
    std::shared_ptr<EvalEnv> parent=nullptr;
    EvalEnv(std::shared_ptr<EvalEnv> parent,
            std::unordered_map<std::string, ValuePtr> SymbolList);

public:
    EvalEnv();
    EvalEnv(const EvalEnv& p) = default;
    void setParent(std::shared_ptr<EvalEnv> parent);
    std::shared_ptr<EvalEnv> getParent();
    std::unordered_map<std::string, ValuePtr> SymbolList;
    void defineBinding(std::string name, ValuePtr args);
    ValuePtr eval(ValuePtr expr);
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    ValuePtr lookupBinding(std::string name);
    std::shared_ptr<EvalEnv> createChild(const std::vector<std::string>& params,
                                         const std::vector<ValuePtr>& args);

};
#endif 
