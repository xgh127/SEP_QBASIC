#include "program.h"
#include<QDebug>
program::program()
{
head=new statement();
proLength=0;
}
void program::clean()
{
    statement *p=head->next;
    statement *q;
    head->next=nullptr;
    while(p!=nullptr)
    {
        q=p->next;
        delete p;
        p=q;
    }
    proLength=0;
}
int program::getLenghth(){return proLength;}
statement* program::move(int lNum)
{
    statement *p=head->next;
    statement *q=head;
    //这里是为了找到待插入代码行号前面的一行代码
    if(head->next!=nullptr)
    {
        while(p->lineNum<lNum && p->next!=nullptr)
        {
              p=p->next;
              q=q->next;
        }//找到第一个比他大的代码
        if(p->lineNum==lNum)    //代表正好有这一行代码，设置成替换，并且返回这一行
        {
              state=0;
              return p;
        }
        else if(p->lineNum<lNum) //插入在后面
        {
            state=1;
            return p;
        }
         else//如果插入在p的前面
            {
            state=1;
            return q;
        }

    }
    else                      //如果第一句都没有
    {
        state=1;
        return head;
    }
}
void program::insert(int lNum, QString nextText)
{

    statement* tmp=move(lNum);
    statement* newStatement;
    if(state==0)              //替换之
    {
        tmp->text=nextText;
    }
    else
    {
           newStatement=new statement(nextText,tmp->next);
           tmp->next=newStatement;
           proLength++;
    }
}
 program::~program()
{
    statement *p = head->next;
    statement *q = head;
    while(p!=nullptr)
    {
        q = p;
        delete p;
        p = q;
        p = p->next;
    }
}
void program::del(int num)
{
    statement *p = head->next;
    statement *q = head;
    while(p!=nullptr)
    {
        if(p->lineNum == num)
        {
            q->next = p->next;
            delete p;
        }
        p = p->next;
        q = q->next;
    }
}
