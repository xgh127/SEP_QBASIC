#include "tokenizer.h"
#include<QList>
#include<QString>
#include<QDebug>

tokenizer::tokenizer(){}
QStringList tokenizer::ToTokens(QString expression)
{
    int pre=-1;                                 //记录上一个符号所在的位置
    QString tmp;
    Tokens.clear();
    if (expression.contains('\n'))              //去除换行符
    expression=expression.trimmed();
    for(int i=0;i<expression.length();++i)
    {
        if(isOperator(expression[i]))
        {
            if (expression[i] == '*' &&expression[i+1] == '*')//处理**
            {
                tmp=expression.mid(pre+1,i-1-pre);//获取*号前面的表达式
                Tokens.append(tmp);
                pre=i;                            //pre指向第一个*
                tmp=expression.mid(i,2);          //把**添加进去
                Tokens.append(tmp);
                pre=i+1;
                i++;
                continue;
            }
            tmp=expression.mid(pre+1,i-1-pre);
            Tokens.append(tmp);
            Tokens.append(QString(expression[i]));//把运算符添加进去
            pre=i;

        }
        else continue;
    }
    //把最后一个填进去
    if(pre!=-1)
    {
        tmp=expression.mid(pre+1);
        Tokens.append(tmp);
    }
    else Tokens.append(expression);
    return Tokens;

}
bool tokenizer::isOperator(QCharRef ch)
{
    if(ch=="+"||ch=="-"||ch=="*"||ch=="/"||ch=="="||ch=="("||ch==")")
        return true;
    else return false;
}
