#include "statement.h"
#include"exp.h"
#include"mainwindow.h"
#include"ui_mainwindow.h"
#include<QDebug>

statement::statement()
{
    next=nullptr;
}
statement::statement(QString state,statement *n)
{
    text=state;
    next=n;
    lst=text.split(' ',QString::SkipEmptyParts);
    lineNum=lst[0].toInt();

}
void statement::cover(QString nextText)
{
    text=nextText;
    lst=text.split(' ',QString::SkipEmptyParts);
    lineNum=lst[0].toInt();
}
int statement::getLineNum()
{
   return lineNum;
}
QString statement::getText()
{
    return text;
}
Expression* statement::buildTree(QList<QString> elems,tokenizer* Tokenizer,parser* Parser)
{
    QString code;
    int Mode=2; //2代表正常代码，1代表没有行号直接输入
    if (elems[0] == "LET" || elems[0] == "PRINT") Mode=1;
    for(int i=Mode;i<elems.length();++i)
    {
        code+=elems[i];

    }
    QList<QString> tokens;
    tokens=Tokenizer->ToTokens(code);//分割一个表达式获得单个元素
    Expression *val;
       val=Parser->ToTree(tokens);//转化为一个表达式树
    if(val==nullptr)
    {
        QString error="Error";
        throw error;
    }
    return val;
}

