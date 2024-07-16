#include "evalstate.h"
#include<QDebug>
evalstate::evalstate()
{

}
void evalstate::setValue(QString var, int value)
{
    VarList[var]=value;
   // qDebug()<<"count="<<VarList.count(var);
}
int evalstate::getValue(QString var)
{
    return VarList[var];
}
void evalstate::clear()
{
    VarList.clear();
}
bool evalstate::isDefined(QString var)
{
    int flag=VarList.count(var);
    return flag;
}
/*
QString evalstate::Tranverse()
{
    containing="";
    map<QString,int>::iterator it;
    it=VarList.begin();
    while(it!=VarList.end())
    {
        containing+=it->first;
        containing+='\t';
        containing+=QString::number(it->second);
        containing+='\n';
        it++;
    }
    return containing;
}*/
