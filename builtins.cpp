#include"./builtins.h"
#include"./error.h"
#include"./eval_env.h"
#include<cmath>
#include<iostream>
 std::unordered_map<std::string, BuiltinProValue> BuiltinFuncsList{
    {"+",&add}, 
    {"print", &print},
    {"*", &mul},
    {">", &isBigger},
    {"-", &subtract},
    {"/", &divide},
    {"abs", &Abs},
    {"expt", &expt},
    {"quotient", &quotient},
    {"modulo", &modulo},
    {"remainder", &Remainder},
    {"eq?", &eq},
    {"equal?", &equal},
    {"not", &Not},

    {"=", &Equal},
    {"<", &isSmaller},
    {">=", &isEqualBigger},
    {"<=", &isEqualSmaller},
    {"even?", &even},
    {"odd?", &odd},
    {"zero?", &zero},

    {"append", &append},
    {"car", &car},
    {"cdr", &cdr},
    {"cons", &cons},
    {"length", &length},
    {"list", &mylist},
    {"map", &map},
    {"filter", &filter},
    {"reduce", &reduce},

    {"atom?", &atom},
    {"boolean?", &boolean},
    {"integer?", &integer},
    {"list?", &islist},
    {"number?", &number},
    {"null?", &null},
    {"pair?", &pair},
    {"procedure?", &procedure},
    {"string?", &string},
    {"symbol?", &symbol},
    {"apply", &Apply},
    {"display", &display},
    {"displayln", &displayLN},
    {"error", &error},
    {"eval", &myeval},
    {"exit", &Exit},
    {"newline", &newline},
    {"print", &print},
    {"strcat", &Strcat},
    {"strcmp", &Strcmp},
    {"strlen",&Strlen},
    {"strstr", &Strstr},
    {"at", &At}
    
 };
ValuePtr Apply(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 2)
        throw LispError("too many arguments");
    else if (num < 2)
        throw LispError("not enough arguments");
    else {
       
            if (typeid(*params[0]) == typeid(BuiltinProValue)) {
                auto& proc = static_cast<BuiltinProValue&>(*params[0]);
                std::vector<ValuePtr> args=params[1]->toVector();
                return proc.call()(args, env);
            } 
            else if (typeid(*params[0]) == typeid(LambdaValue)) {
                auto& proc = static_cast<LambdaValue&>(*params[0]);
                std::vector<ValuePtr> args = params[1]->toVector();
                return proc.apply(args);
            } 
            else
                throw LispError("not a procedure");
    }
}
ValuePtr display(const std::vector<ValuePtr>& params, EvalEnv& env) {

    int num = params.size();
    if (num > 1)
            throw LispError("too many arguments");
    else if (num < 1)
            throw LispError("not enough arguments");
    else {
        if (typeid(*params[0]) == typeid(StringValue)) {
                std::cout <<static_cast<const StringValue&>(*params[0]).getValue();
                return std::make_shared<NilValue>();
        } 
        else {
                std::string s = params[0]->toString();
                std::cout << s;
                return std::make_shared<NilValue>();
        }
    }
}
ValuePtr displayLN(const std::vector<ValuePtr>& params, EvalEnv& env) {
    int num = params.size();
    if (num > 1)
            throw LispError("too many arguments");
    else if (num < 1)
            throw LispError("not enough arguments");
    else {
            display(params, env);
            std::cout << std::endl;
            return std::make_shared<NilValue>();
    }
}
ValuePtr error(const std::vector<ValuePtr>& params, EvalEnv& env) {
    int num = params.size();
    if (num > 1)
            throw LispError("too many arguments");
    else if (num < 1)
            throw LispError("not enough arguments");
    else {
            /*
            if (typeid(*params[0]) != typeid(NumericValue))
                throw LispError("not a int");
            double temp = static_cast<const NumericValue&>(*params[0]).getValue();
            if (fabs(temp - (int)temp) > 1e-5) throw LispError("not a int");
            */
            throw LispError(params[0]->toString());
    }
}
ValuePtr myeval(const std::vector<ValuePtr>& params, EvalEnv& env) {
    int num = params.size();
    if (num > 1)
            throw LispError("too many arguments");
    else if (num < 1)
            throw LispError("not enough arguments");
    else {
            return env.eval(params[0]);
    }
}
ValuePtr Exit(const std::vector<ValuePtr>& params, EvalEnv& env) {
    int num = params.size();
    if (num > 1)
            throw LispError("too many arguments");
    else if (num < 1)
    {
            std::exit(0);
    }
    else {
        if (typeid(*params[0]) == typeid(NumericValue))
        {
                double x = static_cast<NumericValue&>(*params[0]).getValue();
                if (x - (int)x < 1e-5)
                    std::exit((int)x);
                else
                    throw LispError("not a int");
        } else
                throw LispError("not a int");
    }

}
ValuePtr newline(const std::vector<ValuePtr>& params, EvalEnv& env) {
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}
ValuePtr add(const std::vector<ValuePtr>& params, EvalEnv& env) {
    double result = 0;
    for (const auto& i : params)
    {
        if (!i->isNumber()) {
        
        throw LispError("Cannot add a non-numeric value. ");
        }
        result +=i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr mul(const std::vector<ValuePtr>& params, EvalEnv& env) {

    double result = 1;
    for (const auto& i : params) {
        if (!i->isNumber()) {
        throw LispError("Cannot multiply a non-numeric value. ");
        }
        result *= i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr isBigger(const std::vector<ValuePtr>& params, EvalEnv& env) {

    if (params.size() > 2) throw LispError("too many arguments");
    else
    {
        auto x = params[0];
        auto y = params[1];
        if (typeid(*x) != typeid(NumericValue) || typeid(*y) != typeid(NumericValue))
        throw LispError("cannot compare two non-number");
        else
        {
        if (x->asNumber() > y->asNumber()) return std::make_shared<BooleanValue>(1);
        else
            return std::make_shared<BooleanValue>(0);
        }
    }
}
ValuePtr print(const std::vector<ValuePtr>& params, EvalEnv& env) {

    std::string s{};
    for (const auto i : params)
    {
        s+=i->toString();
    }
    std::cout << s<<std::endl;
    return std::make_shared<NilValue>();
}
ValuePtr subtract(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size()==2)
    {
        auto x = params[0];
        auto y = params[1];
        if (x->isNumber() && y->isNumber())
        {
        return std::make_shared<NumericValue>(x->asNumber() - y->asNumber());
        } 
        else
        throw LispError("cannot subtract non-number");
    }
    else if (params.size() == 1)
    {
        double x = 0;
        auto y = params[0];
        if (y->isNumber())
        {
        return std::make_shared<NumericValue>(x - y->asNumber());
        } else
        throw LispError("cannot subtract non-number");
    } else
        throw LispError("too many or not enough arguments");
}
ValuePtr divide(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    if (params.size() == 2) {
        auto x = params[0];
        auto y = params[1];
        if (x->isNumber() && y->isNumber()) {
        if (y == 0) throw LispError("can not divided by 0");
        else
        return std::make_shared<NumericValue>(x->asNumber() / y->asNumber());
        } else
        throw LispError("cannot divide non-number");
    } else if (params.size() == 1) {
        double x = 1;
        auto y = params[0];
        if (y->isNumber()) {
        if (y == 0) throw LispError("can not divided by 0");
        else
        return std::make_shared<NumericValue>(x / y->asNumber());
        } else
        throw LispError("cannot divide non-number");
    } else
        throw LispError("too many or not enough arguments");
}
ValuePtr Abs(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 1) throw LispError("too many arguments");
    else if (num < 1) throw LispError("not enough arguments");
    else
    {
        auto value = params[0]->asNumber();
        return std::make_shared<NumericValue>(fabs(value));
    }
}
ValuePtr expt(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 2)
        throw LispError("too many arguments");
    else if (num < 2)
        throw LispError("not enough arguments");
    else {
        auto x = params[0]->asNumber();
        auto y = params[1]->asNumber();
        if (x == 0 && y == 0) throw LispError("can not calculate 0^0");
        else
        {
        double value = pow(x, y);
        return std::make_shared<NumericValue>(fabs(value));
        }
    }
}
ValuePtr quotient(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 2)
        throw LispError("too many arguments");
    else if (num < 2)
        throw LispError("not enough arguments");
    else {
        auto x = params[0]->asNumber();
        auto y = params[1]->asNumber();
        if (y == 0)
        throw LispError("can not dividedby 0");
        else {
        double value = x / y;
        double v;
        if (value >= 0)
        v = floor(value);
        else
        v = ceil(value);
        return std::make_shared<NumericValue>(v);
        }
    }
}
ValuePtr modulo(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 2)
        throw LispError("too many arguments");
    else if (num < 2)
        throw LispError("not enough arguments");
    else {
        auto x = params[0]->asNumber();
        auto y = params[1]->asNumber();
        //////////////////////都得是整数！
        if (x - (int)x < 1e-5 && y - (int)y < 1e-5) {
        int step;
        if (y > 0)
        step = 1;
        else if (y < 0)
        step = -1;
        else
        throw LispError("the second argument cannot be 0");
        for (int q = 0; q < fabs(y); q += step)
        {
        if ((abs(q - (int)x) %(int)y) == 0)
            return std::make_shared<NumericValue>(q);
        }
        } 
        else
        throw LispError("is not int");
    }
}
ValuePtr Remainder(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 2)
        throw LispError("too many arguments");
    else if (num < 2)
        throw LispError("not enough arguments");
    else {
        auto x = params[0]->asNumber();
        auto y = params[1]->asNumber();
        if (x - (int)x < 1e-5 && y - (int)y < 1e-5) {
        //////////////////////都得是整数！
        auto k = quotient(params, env)->asNumber();
        double r = x - y * k;
        return std::make_shared<NumericValue>(r);
        } else
        throw LispError("is not int");
    }
}
ValuePtr eq(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 2)
       throw LispError("too many arguments");
    else if (num < 2)
       throw LispError("not enough arguments");
    else {
       auto x = params[0];
       auto y = params[1];
       if (typeid(*x) != typeid(*y)) return std::make_shared<BooleanValue>(0);
       else
       {
        if (typeid(*x) == typeid(NumericValue)) {
        auto xvalue = static_cast<NumericValue&>(*x).getValue();
        auto yvalue = static_cast<NumericValue&>(*y).getValue();
        return std::make_shared<BooleanValue>(xvalue == yvalue);
        } 
        else if (typeid(*x) == typeid(BooleanValue))
        {
        auto xvalue = static_cast<BooleanValue&>(*x).getValue();
        auto yvalue = static_cast<BooleanValue&>(*y).getValue();
        return std::make_shared<BooleanValue>(xvalue == yvalue);
        } 
        else if (typeid(*x) == typeid(SymbolValue)) {
        auto xvalue = static_cast<SymbolValue&>(*x).getName();
        auto yvalue = static_cast<SymbolValue&>(*y).getName();
        return std::make_shared<BooleanValue>(xvalue == yvalue);
        }
        else if (typeid(*x) == typeid(NilValue)) {
        return std::make_shared<BooleanValue>(1);
        } 
        else
        return std::make_shared<BooleanValue>(x == y);/////这里可能不对
       }
    }
}
bool isequal(ValuePtr x, ValuePtr y)
{
    if (typeid(*x) == typeid(*y)) {
       std::vector<ValuePtr> v{x, y};
       if (typeid(*x) == typeid(SymbolValue)) {
        auto xvalue = static_cast<SymbolValue&>(*x).getName();
        auto yvalue = static_cast<SymbolValue&>(*y).getName();
        return xvalue == yvalue;
       } else if (typeid(*x) == typeid(BooleanValue)) {
        auto xvalue = static_cast<BooleanValue&>(*x).getValue();
        auto yvalue = static_cast<BooleanValue&>(*y).getValue();
        return xvalue == yvalue;

       } else if (typeid(*x) == typeid(NumericValue)) {
        auto xvalue = static_cast<NumericValue&>(*x).getValue();
        auto yvalue = static_cast<NumericValue&>(*y).getValue();
        return xvalue == yvalue;
       } 
       else if (typeid(*x) == typeid(StringValue)) {
        auto xvalue = static_cast<StringValue&>(*x).getValue();
        auto yvalue = static_cast<StringValue&>(*y).getValue();
        return xvalue == yvalue;
       } 
       else if (typeid(*x) == typeid(NilValue)) {
        return true;
       }
       else if (typeid(*x) == typeid(PairValue))
       {
        auto xl = static_cast<PairValue&>(*x).getCar();
        auto xr = static_cast<PairValue&>(*x).getCdr();
        auto yl = static_cast<PairValue&>(*y).getCar();
        auto yr = static_cast<PairValue&>(*y).getCdr();
        return isequal(xl, yl) && isequal(xr, yr);
       }
    }
    else
        return false;
}

ValuePtr equal(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 2)
       throw LispError("too many arguments");
    else if (num < 2)
       throw LispError("not enough arguments");
    else {
       auto x = params[0];
       auto y = params[1];
        return std::make_shared<BooleanValue>(isequal(x,y));
    }
}
ValuePtr Equal(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 2)
       throw LispError("too many arguments");
    else if (num < 2)
       throw LispError("not enough arguments");
    else {
       return std::make_shared<BooleanValue>(params[0]->asNumber() ==
                                             params[1]->asNumber());
    }
}
ValuePtr Not(const std::vector<ValuePtr>& params, EvalEnv& env) {
    int num = params.size();
    if (num > 1)
       throw LispError("too many arguments");
    else if (num < 1)
       throw LispError("not enough arguments");
    else {
        if (typeid(*params[0]) == typeid(BooleanValue))
        {
            if (!static_cast<BooleanValue&>(*params[0]).getValue())
            {
        return std::make_shared<BooleanValue>(1);
        } else
        return std::make_shared<BooleanValue>(0);
       } else
        return std::make_shared<BooleanValue>(0);
    }
}
ValuePtr isSmaller(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 2)
       throw LispError("too many arguments");
    else if (num < 2)
       throw LispError("not enough arguments");
    else {
       return std::make_shared<BooleanValue>(params[0]->asNumber() <
                                             params[1]->asNumber());
    }
}
ValuePtr isEqualSmaller(const std::vector<ValuePtr>& params, EvalEnv& env)
{
     int num = params.size();
    if (num > 2)
       throw LispError("too many arguments");
    else if (num < 2)
       throw LispError("not enough arguments");
    else {
       return std::make_shared<BooleanValue>(params[0]->asNumber() <=
                                             params[1]->asNumber());
    }
}
ValuePtr isEqualBigger(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 2)
       throw LispError("too many arguments");
    else if (num < 2)
       throw LispError("not enough arguments");
    else {
       return std::make_shared<BooleanValue>(params[0]->asNumber() >=
                                             params[1]->asNumber());
    }
}
ValuePtr even(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 1)
       throw LispError("too many arguments");
    else if (num < 1)
       throw LispError("not enough arguments");
    else {
       auto value = params[0]->asNumber();
       if (value - (int)value < 1e-5)
       {
        return std::make_shared<BooleanValue>((int)value%2==0);
       } else
        throw LispError("not a int");
    }
}
ValuePtr odd(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 1)
       throw LispError("too many arguments");
    else if (num < 1)
       throw LispError("not enough arguments");
    else {
       auto value = params[0]->asNumber();
       if (value - (int)value < 1e-5) {
        return std::make_shared<BooleanValue>(!((int)value % 2 == 0));
       } else
        throw LispError("not a int");
    }
}
ValuePtr zero(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    int num = params.size();
    if (num > 1)
       throw LispError("too many arguments");
    else if (num < 1)
       throw LispError("not enough arguments");
    else {
       auto value = params[0]->asNumber();
       if (value - (int)value < 1e-5) {
        return std::make_shared<BooleanValue>(value == 0);
       } else
        throw LispError("not a int");
    }
}
ValuePtr atom(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 1)
       throw LispError("too many rguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else
    {
       if (typeid(*params[0]) == typeid(NumericValue)||typeid(*params[0]) == typeid(StringValue)
           ||typeid(*params[0]) == typeid(BooleanValue)||typeid(*params[0]) == typeid(SymbolValue)
               ||typeid(*params[0]) == typeid(NilValue)) return std::make_shared<BooleanValue>(1);
       else
        return std::make_shared<BooleanValue>(0);
    }
}
ValuePtr integer(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 1)
       throw LispError("too many arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       if (typeid(*params[0]) == typeid(NumericValue)) {
        auto num = params[0]->asNumber();
        if (fabs(num-(int)num)<1e-5)return std::make_shared<BooleanValue>(1);
        else return std::make_shared<BooleanValue>(0);
       } else
        return std::make_shared<BooleanValue>(0);
    }
}
bool judgelist(ValuePtr p)
{
    if (typeid(*p) == typeid(PairValue))
    {
       return judgelist(static_cast<const PairValue&>(*p).getCdr());
    }
    else if (typeid(*p) == typeid(NilValue))
    {
       return true;
    }
    else return false;
}

ValuePtr islist(const std::vector<ValuePtr>& params, EvalEnv& env) {
    //std::vector<ValuePtr> inner{params};
    if (params.size() > 1)
       throw LispError("too many  arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       if (judgelist(params[0]))
        return std::make_shared<BooleanValue>(1);
       else
        return std::make_shared<BooleanValue>(0);
    }
}
ValuePtr boolean(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 1)
       throw LispError("too many arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       if (typeid(*params[0]) == typeid(BooleanValue)) {
        return std::make_shared<BooleanValue>(1);
       } else
        return std::make_shared<BooleanValue>(0);
    }
}
ValuePtr number(const std::vector<ValuePtr>& params, EvalEnv& env)
{
    if (params.size() > 1)
       throw LispError("too many arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       if (typeid(*params[0]) == typeid(NumericValue)) {
        return std::make_shared<BooleanValue>(1);
       } else
        return std::make_shared<BooleanValue>(0);
    }
}
ValuePtr null(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 1)
       throw LispError("too many arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       if (typeid(*params[0]) == typeid(NilValue)) {
        return std::make_shared<BooleanValue>(1);
       } else
        return std::make_shared<BooleanValue>(0);
    }
}
ValuePtr pair(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 1)
       throw LispError("too many arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       if (typeid(*params[0]) == typeid(PairValue)) {
        return std::make_shared<BooleanValue>(1);
       } else
        return std::make_shared<BooleanValue>(0);
    }
}
ValuePtr procedure(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 1)
       throw LispError("too many arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       if (typeid(*params[0]) == typeid(BuiltinProValue) ||
           typeid(*params[0]) == typeid(LambdaValue)) {
        return std::make_shared<BooleanValue>(1);
       } else
        return std::make_shared<BooleanValue>(0);
    }
}
ValuePtr string(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 1)
       throw LispError("too many arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       if (typeid(*params[0]) == typeid(StringValue)) {
        return std::make_shared<BooleanValue>(1);
       } else
        return std::make_shared<BooleanValue>(0);
    }
}
ValuePtr symbol(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 1)
       throw LispError("too many arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       if (typeid(*params[0]) == typeid(SymbolValue)) {
        return std::make_shared<BooleanValue>(1);
       } else
        return std::make_shared<BooleanValue>(0);
    }
}
ValuePtr append(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() == 0)
       return std::make_shared<NilValue>();
    else if (params.size() >= 1) {

      std::vector<ValuePtr> AddAll;
      for (auto i = params.begin(); i != params.end(); i++)
      {
        std::vector<ValuePtr> temp{*i};
        if (static_cast<BooleanValue&>(*islist(temp, env)).getValue()) {
        auto v = (*i)->toVector();
        for (auto j = v.begin(); j != v.end(); j++){
            AddAll.push_back(*j);
        }
        } 
        else
        throw LispError("not a list");
      }
      return mylist(AddAll, env);
    }
}
ValuePtr car(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 1)
       throw LispError("too many arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       if (typeid(*params[0]) == typeid(PairValue)) {
        auto& pair = static_cast<PairValue&>(*params[0]);
        return pair.getCar();
       } else
        throw LispError("not a pair");  
    }
}
ValuePtr cdr(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 1)
       throw LispError("too many arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       if (typeid(*params[0]) == typeid(PairValue)) {
        auto& pair = static_cast<PairValue&>(*params[0]);
        return pair.getCdr();
       } else
        throw LispError("not a pair");
    }
}
ValuePtr cons(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 2)
       throw LispError("too many  arguments");
    else if (params.size() <2 )
       throw LispError("not enough arguments");
    else {
       return std::make_shared<PairValue>(params[0], params[1]);        
    }
}
ValuePtr length(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() >1)
       throw LispError("too many  arguments");
    else if (params.size()==0)
       throw LispError("not enough arguments");
    else {
       auto& result = static_cast<BooleanValue&>(*islist(params, env));
       if (result.getValue()) {
        return std::make_shared<NumericValue>(params[0]->toVector().size());
       } else
        throw LispError("not a list");
    }
}
ValuePtr mylist(const std::vector<ValuePtr>& params, EvalEnv& env) {//创建列表
    if (params.size() == 0)
       return std::make_shared<NilValue>();
    else
    {
       std::shared_ptr<PairValue> p = std::make_shared<PairValue>(*(-- params.end()), std::make_shared<NilValue>());
       if (params.size() == 1) return p;
       for (auto i = --(--params.end());; i--)
       {
        p = std::make_shared<PairValue>(*i, p);
        if (i == params.begin()) break;
       }
       return p;
    }
}
ValuePtr map(const std::vector<ValuePtr>& params, EvalEnv& env) {
    //proc应能接受一个实参
    if (params.size() > 2)
       throw LispError("too many  arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       std::vector<ValuePtr> List,answer;
       auto r = islist(std::vector<ValuePtr>{params[1]}, env);
       if (!static_cast<const BooleanValue&>(*r).getValue())
       {
        throw LispError("not a list");
       }
       List.push_back(params[1]);
       auto& result = static_cast<BooleanValue&>(*(islist(List, env)));
       if (result.getValue()) {
           List = params[1]->toVector();
           if (typeid(*params[0]) == typeid(BuiltinProValue)) {
           auto& proc = static_cast<BuiltinProValue&>(*params[0]);
               for (auto i = List.begin(); i != List.end(); i++)
               {
                std::vector<ValuePtr> args;
                args.push_back(*i);
                auto a = proc.call()(args,env);
                answer.push_back(a);
               }
               return mylist(answer, env);
           }
           else if (typeid(*params[0]) == typeid(LambdaValue))
           {
               auto& proc = static_cast<LambdaValue&>(*params[0]);
               for (auto i = List.begin(); i != List.end(); i++) {
                std::vector<ValuePtr> args;
                args.push_back(*i);
                auto a = proc.apply(args);
                answer.push_back(a);
               }
               return mylist(answer, env);
           }
           else throw LispError("not a procedure");
       }
       else
           throw LispError("not a list");
    }
}
ValuePtr filter(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 2)
       throw LispError("too many  arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       std::vector<ValuePtr> List, answer;
       List.push_back(params[1]);
       auto& result = static_cast<BooleanValue&>(*islist(List, env));
       if (result.getValue()) {
           std::vector<ValuePtr>List2 = params[1]->toVector();
           if (typeid(*params[0]) == typeid(BuiltinProValue)) {
               auto& proc = static_cast<BuiltinProValue&>(*params[0]);

               for (auto i = List2.begin(); i != List2.end(); i++) {
                std::vector<ValuePtr> args;
                args.push_back(*i);
                auto a = proc.call()(args, env);
                if (typeid(*a) == typeid(BooleanValue)) {
                    if (static_cast<BooleanValue&>(*a).getValue()) {
                        answer.push_back(*i);
                    }
                }
               }
               return mylist(answer, env);
           } else if (typeid(*params[0]) == typeid(LambdaValue)) {
               auto& proc = static_cast<LambdaValue&>(*params[0]);
               for (auto i = List2.begin(); i != List2.end(); i++) {
                std::vector<ValuePtr> args;
                args.push_back(*i);
                auto a = proc.apply(args);
                if (typeid(*a) == typeid(BooleanValue)) {
                    if (static_cast<BooleanValue&>(*a).getValue()) {
                        answer.push_back(*i);
                    }
                }
                //answer.push_back(*i);
               }
               return mylist(answer, env);
           } else
               throw LispError("not a procedure");
       } else
           throw LispError("not a list");
    }
}
ValuePtr reduce(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 2)
       throw LispError("too many  arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       std::vector<ValuePtr> List, answer;
       List.push_back(params[1]);
       auto& result = static_cast<BooleanValue&>(*islist(List, env));
       if (result.getValue() && !params[1]->toVector().empty()) {
           List = params[1]->toVector();
           if (List.size() == 1)
           {
               std::vector<ValuePtr> temp{params[1]};
               return car(temp, env);
           }
           else
           {
               std::vector<ValuePtr> temp{params[1]};
               auto Cdr = cdr(temp, env);
               auto Car = car(temp, env);
               std::vector<ValuePtr> args1{params[0], Cdr};
               auto part2 = reduce(args1, env);
               std::vector<ValuePtr> args2{ params[0],Car, part2};
               return env.eval(mylist(args2,env));
           }
       } else
           throw LispError("not a non-empty list");
    }
}
ValuePtr WHen(const std::vector<ValuePtr>& params, EvalEnv& env) {
     if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       ValuePtr result = env.eval(params[0]);
       if (typeid(*result) == typeid(BooleanValue)) {
           if (static_cast<const BooleanValue&>(*result).getValue()) {
               for (auto i = (++params.begin()); i != params.end(); i++) {
                result = env.eval(*i);
               }
               return result;
           }
           return result;
       } else {
           throw LispError("invalid condition");
       }
    }
}
ValuePtr UNless(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       ValuePtr result = env.eval(params[0]);
       if (typeid(*result) == typeid(BooleanValue)) {
           if (!static_cast<const BooleanValue&>(*result).getValue()) {
               for (auto i = (++params.begin()); i != params.end(); i++) {
                result = env.eval(*i);
               }
               return result;
           }
           return result;
       } else {
           throw LispError("invalid condition");
       }
    }
}
ValuePtr Strcat(const std::vector<ValuePtr>& params, EvalEnv& env) // 字符串拼接
{
    if (params.size() > 2)
       throw LispError("too many  arguments");
    else if (params.size() == 0)
       throw LispError("not enough arguments");
    else {
       if (typeid(*params[0]) == typeid(StringValue) &&
           typeid(*params[1]) == typeid(StringValue))
        {
           std::string s1 = static_cast<const StringValue&>(*params[0]).getValue();
           std::string s2 = static_cast<const StringValue&>(*params[1]).getValue();
           return std::make_shared<StringValue>(s1 + s2,1);

        } else
           throw LispError("not a string");
    }
}
ValuePtr Strcmp(const std::vector<ValuePtr>& params, EvalEnv& env) // 字符串比较
{
     if (params.size() > 2)
        throw LispError("too many  arguments");
    else if (params.size() == 0)
        throw LispError("not enough arguments");
    else {
        if (typeid(*params[0]) == typeid(StringValue) &&
            typeid(*params[1]) == typeid(StringValue)) {
           std::string s1 =
               static_cast<const StringValue&>(*params[0]).getValue();
           std::string s2 =
               static_cast<const StringValue&>(*params[1]).getValue();
           if (s1 > s2) return std::make_shared<NumericValue>(1);
           else if (s1 == s2)
               return std::make_shared<NumericValue>(0);
           else
               return std::make_shared<NumericValue>(-1);
        } else
           throw LispError("not a string");
    }
}
ValuePtr Strlen(const std::vector<ValuePtr>& params, EvalEnv& env)  // 字符串长度
{
    if (params.size() > 1)
        throw LispError("too many  arguments");
    else if (params.size() == 0)
        throw LispError("not enough arguments");
    else {
        if (typeid(*params[0]) == typeid(StringValue)) {
           std::string s1 =
               static_cast<const StringValue&>(*params[0]).getValue();
           return std::make_shared<NumericValue>(s1.length());

        } else
           throw LispError("not a string");
    }
}
ValuePtr Strstr(const std::vector<ValuePtr>& params, EvalEnv& env) // 字符串查找
{
    if (params.size() > 2)
        throw LispError("too many  arguments");
    else if (params.size() == 0)
        throw LispError("not enough arguments");
    else {
        if (typeid(*params[0]) == typeid(StringValue) &&
            typeid(*params[1]) == typeid(StringValue)) {
           std::string s1 =static_cast<const StringValue&>(*params[0]).getValue();
           std::string s2 =static_cast<const StringValue&>(*params[1]).getValue();
           auto result = s1.find(s2, 0);
           if (result != std::string::npos) {
               return std::make_shared<NumericValue>(result);
           } else
               return std::make_shared<BooleanValue>(0);
           return std::make_shared<BooleanValue>(s1 > s2);

        } else
           throw LispError("not a string");
    }
}
ValuePtr At(const std::vector<ValuePtr>& params, EvalEnv& env)// 下标访问
{
    if (params.size() > 2)
        throw LispError("too many  arguments");
    else if (params.size() == 0)
        throw LispError("not enough arguments");
    else {
        if (typeid(*params[0]) == typeid(StringValue) &&
            typeid(*params[1]) == typeid(NumericValue)) {
           std::string s1 =
               static_cast<const StringValue&>(*params[0]).getValue();
           double i=
               static_cast<const NumericValue&>(*params[1]).getValue();
           if (fabs(i - (int)i) < 1e-5)
           {
               if (i > s1.length() - 1||i<0) throw LispError("out of range");
               return std::make_shared<SymbolValue>(std::string{s1[i]});
           }
           else
           throw LispError("not a int");

        } else
           throw LispError("invalid arguments");
    }
}
ValuePtr Set(const std::vector<ValuePtr>& params, EvalEnv& env) // 给已定义的变量赋值
{
    if (params.size() > 2)
        throw LispError("too many  arguments");
    else if (params.size() == 0)
        throw LispError("not enough arguments");
    else {
        if (typeid(*params[0]) == typeid(SymbolValue)) {
           auto name = static_cast<const SymbolValue&>(*params[0]).getName();
           if (env.SymbolList.find(name) != env.SymbolList.end())
           {
           env.defineBinding(name, params[1]);
           }
           else
           {
           throw LispError("varible not defined");
           }
           
        } 
        else
           throw LispError("not a varible name");
    }
}
