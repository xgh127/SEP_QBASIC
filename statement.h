#ifndef STATEMENT_H
#define STATEMENT_H
# include<QString>
# include<string.h>
# include<QList>
#include"tokenizer.h"
#include"parser.h"
#include"exp.h"
using namespace std;
class statement
{
    friend class program;
    friend class MainWindow;
public:
    statement();
    statement(QString state,statement*n = nullptr);
    void cover(QString ntext);
    int getLineNum();
    QString getText();
    Expression* buildTree(QList<QString> elems,tokenizer* Tokenizer,parser* Parser);

private:
    int lineNum = 0;
    QString text;
    statement *next;
    QList<QString> lst;
};
/*
class LetStmt:public statement
{

public:
     Expression* bulidTree(QList<QString> elems,tokenizer* Tokenizer,parser* Parser);
     //QString displayTree(Expression *Tree,QList<QString> dis);
};
class PrintStmt:public statement
{
public:
    Expression* buildTree(QList<QString> elems,tokenizer* Tokenizer,parser* Parser);
};*/

#endif // STATEMENT_H
