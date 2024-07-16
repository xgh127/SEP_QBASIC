#include "parser.h"
#include<QDebug>

parser::parser(){}
Expression* parser::ToTree(QList<QString> tokens)
{


    if(tokens[0]=="-") tokens.insert(0,"0");
    //处理负数的情况
    for(int i=0;i<tokens.length();++i)
    {
        if(tokens[i]=="") tokens.removeAt(i);
    }
    int num=tokens.length();
    for(int i=0;i<num;++i)
    {

        if((tokens[i]=="-"&&tokens[i-1]=="=")||(tokens[i]=="-"&&tokens[i-1]=="("))
        {
            tokens.insert(i,"0");        //处理负数
            i++;
        }
    }
   QString tmp;
  /* for(int i=0;i<tokens.length();++i)
   {
       qDebug()<<tokens[i];
   }*/
   for(int i=0;i<tokens.length();++i)
   {
       tmp=tokens[i];
       if(isConstant(tmp))              //常数直接进栈
       {
           ConstantExp *c=new ConstantExp(tmp.toInt());
           Operands.push(c);
           continue;
       }
       else if(isIdentifer(tmp))             //变量也直接进栈
       {
           IdentifierExp *I=new IdentifierExp(tmp);
           Operands.push(I);
           continue;
       }
       else if(isCompound(tmp))              //复合型表达式
       {
           if(isLeftPar(tmp))           //如果是左括号
           {
               Operators.push(tmp);
               continue;
           }
           else if(isRightPar(tmp))     //如果是右括号
           {
               while(Operators.top()!="(")
               {
                   Operands.push(toCompExp());
               }
               Operators.pop();         //左括号出栈
           }
           else
           {
               if(Operators.empty())
                  Operators.push(tmp);
               else
               {
                    int topPri=getPriority(Operators.top());
                    int curpri=getPriority(tmp);
                    if (curpri > topPri)
                           Operators.push(tmp);
                    else
                        {
                        if (curpri == 4 && topPri == 4)                 //如果遇到连续的乘方
                             {
                               Operators.push(tmp);                     //直接进栈
                             }
                        else if (topPri == 4 && curpri<4)
                             {
                                while(getPriority(Operators.top())==4) //直到运算符栈顶不是**
                              {
                                        Operands.push(toCompExp());
                                }
                                i--;
                                continue;
                             }
                        else                                            //其它情况
                            {
                                Operands.push(toCompExp());
                                Operators.push(tmp);
                            }
                        }

               }
           }
       }
       else {
           return nullptr;
}
   }
   while(!Operators.empty())                                            //把操作符栈清空
   {
       Operands.push(toCompExp());
   }
     Expression* root=nullptr;
     root=Operands.top();
     return root;
}
Expression* parser::getTop(stack<Expression*> s)
{
    if(!s.empty())
    {
        Expression* tmp=s.top();
        s.pop();
      return  tmp;
    }
    else return nullptr;
}
CompoundExp* parser::toCompExp()
{
    QString top=Operators.top();
    Operators.pop();
    Expression* right=getTop(Operands);

    Operands.pop();
    Expression* left=getTop(Operands);
    if(Operands.empty()) return nullptr;
    Operands.pop();
    CompoundExp* cp=new CompoundExp(top,left,right);
    return cp;
}
bool parser::isCompound(QString str)
{
    if(isEqual(str)||isSubAdd(str)||isDivMul(str)||isLeftPar(str)||isRightPar(str)||str == "**")
        return true;
     return false;
}
bool parser::isConstant(QString str)
{
    bool ok;
    str.toInt(&ok);
    return ok;
}
bool parser::isIdentifer(QString str)
{
    if((str[0]>="A"&&str[0]<="Z")||(str[0]>="a"&&str[0]<="z"))
        return true;
    else return false;
}
bool parser::isEqual(QString str)
{
    return str=="=";
}
bool parser::isSubAdd(QString str)
{
    return (str=="+"||str=="-");
}
bool parser::isDivMul(QString str)
{
    return (str=="*"||str=="/");
}
bool parser::isLeftPar(QString str)
{
    return str=="(";
}
bool parser::isRightPar(QString str)
{
    return str==")";
}
int parser::getConstValue(QString str)
{
    return str.toInt();
}
int parser::getIdentifierValue(QString exp, evalstate &Vars)
{
    return Vars.getValue(exp);
}
int parser::getPriority(QString op)
{
    if(isEqual(op)) return 1;
    if(isSubAdd(op)) return 2;
    if(isDivMul(op)) return 3;
    if(op == "**") return 4;
    return 0;//异常返回0
}
