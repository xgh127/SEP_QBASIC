#ifndef PARSER_H
#define PARSER_H
#include"exp.h"
#include<stack>
using namespace  std;

class parser
{
public:
    parser();
    Expression* ToTree(QList<QString> tokens);
private:
    stack<QString> Operators;
    stack<Expression*> Operands;
    //下面是一堆判断函数
    bool isCompound(QString);
    bool isConstant(QString);
    bool isIdentifer(QString);
    int getConstValue(QString);
    int getIdentifierValue(QString exp,evalstate &Vars);
    int getPriority(QString op);
    bool isSubAdd(QString);
    bool isDivMul(QString);
    bool isEqual(QString);
    bool isLeftPar(QString);
    bool isRightPar(QString);
    Expression* getTop(stack<Expression*> s);
    CompoundExp* toCompExp();
};

#endif // PARSER_H
