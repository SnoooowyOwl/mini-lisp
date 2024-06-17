#ifndef VALUE_H
#define VALUE_H
#include"./token.h"
#include"./color.h"
//#include"./eval_env.h"
#include<string>
#include<sstream>
#include<iomanip>
#include<vector>
class EvalEnv;
class Value;
using ValuePtr = std::shared_ptr<Value>;
class Value
{
public:
    virtual ~Value() = default;
    virtual std::string toString() = 0;//外部表示
    std::vector<ValuePtr> toVector();
    bool isNil();
    bool isSelfEvaluating();
    bool isNumber();
    double asNumber();
    std::optional<std::string> asSymbol();
};

class BooleanValue :public Value
{
    const bool value;
    public:
    bool getValue() const;
    BooleanValue(bool v) : value{v}{}
    std::string toString();
};
class NumericValue : public Value {
      const double value;
    public:
      double getValue() const;
      std::string toString();
      NumericValue(double x) : value{x}{}
};
class StringValue : public Value {
        std::string value;
    public:
        std::string getValue()const;
        StringValue(const std::string& c);
      std::string toString();
        StringValue(std::string s,bool raw);
};
class NilValue : public Value {
    public:
       std::string toString();
};
class SymbolValue : public Value {
     const  std::string value;
   public:
     std::string getName() const;
        SymbolValue(std::string c) : value {c}{ }
    std::string toString();
};
class PairValue : public Value {
   const std::shared_ptr<Value> lvalue;
   const std::shared_ptr<Value> rvalue;
    public:
    std::shared_ptr<Value> getCar() const;
    std::shared_ptr<Value> getCdr() const;
        PairValue(std::shared_ptr<Value> l, std::shared_ptr<Value> r): lvalue{l}, rvalue {r}{}
    std::string toString();
};
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&,EvalEnv& env);
class BuiltinProValue : public  Value {

    BuiltinFuncType* func = nullptr;

public:
     BuiltinFuncType* call() const;
    std::string toString();//const override
        BuiltinProValue(BuiltinFuncType f);
};
class LambdaValue : public Value {
        std::vector<std::string> params;
        std::vector<ValuePtr>body;
        std::shared_ptr<EvalEnv> environment;
    public:
        std::string toString();
        LambdaValue();
        ValuePtr apply(const std::vector<ValuePtr>& args);
        //std::shared_ptr<EvalEnv> getEnvi();
        //LambdaValue(std::vector<std::string> p, std::vector<ValuePtr> b);
        LambdaValue(std::vector<std::string> p, std::vector<ValuePtr> b,
                    std::shared_ptr<EvalEnv> envi);
};
#endif
