#include "exp.h"
#include<QDebug>
#include<cmath>
Expression::Expression(){}
Expression::~Expression(){}
int ConstantExp::getValue()
{
    return value;
}
ConstantExp::ConstantExp(int val){value=val;}
QString ConstantExp::getIdentifierName(){return " ";}
int ConstantExp::eval(evalstate &Vars)
{
    return value;
}
ExpressionType ConstantExp::type(){return CONSTANT;}
ConstantExp::~ConstantExp(){}
IdentifierExp::IdentifierExp(QString name)
{
    this->name=name;
}
ExpressionType IdentifierExp::type(){return IDENTIFIER;}
QString IdentifierExp::getIdentifierName()
{
    return this->name;
}
int IdentifierExp::eval(evalstate &Vars)
{

    if(!Vars.isDefined(name))
    {
        QString error="";
        error+="Error!";
        throw error;
    }
    else
    {
        return Vars.getValue(name);
    }

}

IdentifierExp::~IdentifierExp(){}
CompoundExp::CompoundExp(QString op,Expression *lhs,Expression *rhs)
{
    this->op=op;
    this->lhs=lhs;
    this->rhs=rhs;
}
CompoundExp::~CompoundExp(){}
ExpressionType CompoundExp::type(){return COMPOUND;}
//这是CompundExp计算的函数
int CompoundExp::eval(evalstate &Vars)
{
    if(rhs == nullptr)
    {
        QString t = "Illegal operator in expression , an operator must have right value!";
        throw t;
    }
    int right = rhs->eval(Vars);
       if (op == "=") {//如果是个等于，设置变量的值
           QString variables=lhs->getIdentifierName();
          Vars.setValue(variables, right);
          return right;
       }
       if(lhs == nullptr)
       {
           QString t = "Illegal operator in expression , an operator must have left value!";
           throw t;
       }
       int left = lhs->eval(Vars);
       if (op == "+") return left + right;
       else if (op == "-") return left - right;
       else if (op == "*") return left * right;
       else if (op == "/") {
         if (right == 0)
          {
              QString error = "Divided by 0";
              throw error;
          }
          return left / right;
       }
       else if (op == "**") {
            return pow(left,right);
       }
       else
       {
           QString err = "Illegal operator in expression";
            throw err;
       }

}
QString CompoundExp::getIdentifierName(){return " ";}
Expression* CompoundExp::getLHS(){return lhs;}
Expression* CompoundExp::getRHS(){return rhs;}
QString CompoundExp::getOperator(){return op;}
QString CompoundExp::getOp()
{
    return op;
}


