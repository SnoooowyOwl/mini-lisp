#include<string>
#include<iomanip>
#include"./token.h"
#include"./value.h"
#include"./error.h"
#include"./eval_env.h"
#include<memory>
#include<iostream>
bool Value::isNumber() {
    if (typeid(*this) == typeid(NumericValue))
        return true;
    else
        return false;
}
double Value::asNumber()
{
    if (typeid(*this) == typeid(NumericValue)) {
        return static_cast<const NumericValue&>(*this).getValue();
    } else {
        throw LispError("not a numeric value. ");
    }
}
std::vector<ValuePtr> Value::toVector()
{
    std::vector<ValuePtr> v;
    if (typeid(*this) == typeid(PairValue))
    {
        auto& pair = static_cast<const PairValue&>(*this);
        v.push_back(pair.getCar());
        auto subv = pair.getCdr()->toVector();
            for (auto it = subv.begin(); it != subv.end(); it++)
            {
                v.push_back(*it);
            }
        return v;
    }
    if (typeid(*this) == typeid(SymbolValue)) {
        ValuePtr p = std::make_shared<SymbolValue>(static_cast<const SymbolValue&>(*this).getName());
        v.push_back(p);
        return v;
    }
    if (typeid(*this) == typeid(NumericValue)) {
        ValuePtr p = std::make_shared<NumericValue>(static_cast<const NumericValue&>(*this).getValue());
        v.push_back(p);
        return v;
    }
    if (typeid(*this) == typeid(BooleanValue)) {
        ValuePtr p = std::make_shared<BooleanValue>(static_cast<const BooleanValue&>(this).getValue());
        v.push_back(p);
        return v;
    }
    else
    {
        return v;
    }
}
std::optional<std::string> Value::asSymbol() {
    if (typeid(*this) == typeid(SymbolValue))
    {
        return static_cast<const SymbolValue&>(*this).getName();
    }
    else
    {
        return std ::nullopt;
    }
}
bool Value::isNil() {
    if (typeid(*this) == typeid(NilValue))
        return true;
    else
        return false;
}
bool Value::isSelfEvaluating()
{
    if (typeid(*this) == typeid(BooleanValue) ||
        typeid(*this) == typeid(StringValue) || 
        typeid(*this) == typeid(NumericValue)|| 
        typeid(*this) == typeid(BuiltinProValue))
        return true;
    else
        return false;
}
std::string BooleanValue::toString()
{
    if (value)
        return "#t";
    else
        return "#f";
}
bool BooleanValue::getValue() const
{
    return value;
}
std::string NumericValue::toString()
{
    std::string s; 
    if (fabs(value - (int)value) < 1e-5)
        s = std::to_string((int)value);
    else s = std::to_string(value);
    return s;
}
double NumericValue::getValue() const {
    return value;
}
StringValue::StringValue(std::string s, bool raw)
{
    value = s;
}
StringValue::StringValue(const std::string& c) {
    std::stringstream sin;
    sin << std::quoted(c);
    value = sin.str().substr(1, sin.str().length() - 2);
}
std::string StringValue::getValue() const {
    return value;
}
std::string SymbolValue::getName() const {
    return value;
}
std::string StringValue::toString()
{
   std::string s{value};
    return ("\""+s+"\"");
}
std::string SymbolValue::toString()
{
    return value;
}
std::string NilValue::toString() 
{
    return "()";
}
std::shared_ptr<Value> PairValue::getCar()const
{
    return lvalue;
}
std::shared_ptr<Value> PairValue::getCdr()const
{
    return rvalue;
}
std::string PairValue::toString()
{
    std::string left = lvalue->toString(),right;
    if (typeid(*rvalue) == typeid(PairValue)) 
    { 
      auto& pair = static_cast<const PairValue&>(*rvalue);
        std::string s = rvalue->toString();
      std::string s1 = s.substr(1,s.length() - 2);
        return "(" + left + " " +s1 + ")";
    }
    else if (typeid(*rvalue) == typeid(NilValue))
    {
        return "(" + left + ")";
    }
    else
    {
        right = rvalue->toString();
        return "(" + left + " . " +right  + ")";
    }
}
BuiltinProValue::BuiltinProValue(BuiltinFuncType f) : func{f} {}
std::string BuiltinProValue::toString()
{
    return "#Procedure";
}
BuiltinFuncType* BuiltinProValue::call() const{
    return func;
}
std::string LambdaValue::toString()
{
    return "#Procedure";
}
LambdaValue::LambdaValue(std::vector<std::string> p, std::vector<ValuePtr> b,
                         std::shared_ptr<EvalEnv> envi)
    : params{p}, body{b}, environment{envi} {}
LambdaValue::LambdaValue() {}
ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args) {
    auto newEnvi = environment->createChild(params, args);

    ValuePtr result;
    for (auto i = body.begin(); i != body.end(); i++) {
        result = newEnvi->eval(*i);
    }
    return result;
}



