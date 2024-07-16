#ifndef PROGRAM_H
#define PROGRAM_H
#include<QList>
#include"statement.h"

class program
{
public:
    program();
    ~program();
    void clean();
    int getLenghth();
    statement* move(int lNum);
    void insert(int lNum,QString nextText);
    QString Visit(int lNum);
    void del(int lNum);
   // statement* findLine(int lNum);
     statement *head;

private:
    int proLength=0;
    bool state=1;


};

#endif // PROGRAM_H
