#ifndef TOKENIZER_H
#define TOKENIZER_H
#include<QString>
#include<QList>

using namespace std;

class tokenizer
{
public:
    tokenizer();
    QStringList ToTokens(QString expression);
    bool isOperator(QCharRef ch);
    bool isCom(QString ch);
    bool isConstantExp(QString ch);

private:
    QList<QString> Tokens;
};

#endif // TOKENIZER_H
