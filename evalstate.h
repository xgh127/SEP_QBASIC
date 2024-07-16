#ifndef EVALSTATE_H
#define EVALSTATE_H
#include<QString>
#include<map>
using namespace std;

class evalstate
{
public:
    evalstate();
    void setValue(QString var,int value);//用于增删或者修改变量的值
    int getValue(QString var);
    bool isDefined(QString var);//检查是否重定义
    void clear();
   // QString Tranverse();//遍历
    private:
    map<QString,int> VarList;
   // QString containing;
};

#endif // EVALSTATE_H
