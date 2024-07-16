#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"tokenizer.h"
#include"exp.h"
#include<string>
#include<QDebug>
#include<QFile>
#include<QFileDialog>
#include<QList>
#include<QString>
#include<QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     Tokenizer=new tokenizer;
     Parser=new parser;
     pro=new program();
     directCode=new statement;
     connect(ui->btnClearCode,SIGNAL(clicked()),this,SLOT(clearAll()));
     connect(ui->btnRunCode,SIGNAL(clicked()),this,SLOT(RunCode()));
     connect(ui->btnLoadCode,SIGNAL(clicked()),this,SLOT(loadCode()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_cmdLineEdit_editingFinished()
{
    QString cmd = ui->cmdLineEdit->text();
    if (cmd == "") return;          //为了解决和按钮的冲突，如果移开鼠标去点击按钮，这个函数也会被调用，且输入为空，会导致越界错误
    ui->cmdLineEdit->setText("");
    if (cmd == "CLEAR")
    {
        clearAll();
        return;
    }
    else if (cmd == "QUIT") {this->close();return;}
    else if (cmd == "LOAD") {loadCode();return;}
    else if (cmd == "RUN")  {RunCode();return;}
    else if (cmd == "HELP")
    {
        QString help="自助者天助之";
        ui->textBrowser->append(help);
    }
    else                            //如果是输入的代码
    {
    QString str=cmd.trimmed();      //将输入转化为QString类型，以便于后续处理
    QStringList TokenList=str.split(' ',QString::SkipEmptyParts);//除掉空格之类的
    if(TokenList[0]=="?")           //处理input
    {
        int value=TokenList[2].toInt();
        Vars.setValue(TokenList[1],value);
        RunPro(RunAt);
        return;
    }
    if (TokenList[0] == "LET")
    {
        Expression *val;
        val=directCode->buildTree(TokenList,Tokenizer,Parser);//转化为一个表达式树
        try {
             val->eval(Vars);
        } catch (QString error) {
            ui->treeDisplay->append(error);
            return;
        }
        return;
    }
    if (TokenList[0]=="INPUT")
    {
        QString variable=TokenList[1].trimmed();
        QString str="? "+variable+" ";
        ui->cmdLineEdit->setText(str);
        return;
    }
    if (TokenList[0] == "PRINT")
    {
        Expression *val;
        val=directCode->buildTree(TokenList,Tokenizer,Parser);//转化为一个表达式树
        int value =0;
        try {
            value = val->eval(Vars);
        } catch (QString error) {
            ui->treeDisplay->append(error);
            return;
        }
       QString num=QString::number(value);
        ui->textBrowser->append(num);
        return;
    }
    int lineNum=TokenList[0].toInt(); //获得行号
    if(TokenList.length()==1)         //只有一个数字那就是删除对应的代码
    {
        pro->del(lineNum);       //删除对应行号的代码
        displayCode(pro);        //更新代码展示区
        return;                  //结束
    }
    pro->insert(lineNum,str);    //插入到pro中并且完成排序
    displayCode(pro);            //显示代码
    return;                      //结束
    }
}
void MainWindow::displayCode(program *pro)
{
    statement* stmt=pro->head->next;
     ui->CodeDisplay->clear();
    while(stmt!=nullptr)
    {
         ui->CodeDisplay->append(stmt->getText());
         stmt=stmt->next;
    }
}
void MainWindow::clearAll()
{
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
    pro->clean();
}
void MainWindow::loadCode()
{
    QString path= QFileDialog::getOpenFileName(this,"打开文件","C:\\Users\\xuguohong\\Desktop\\private documentary\\se\\testcases");
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd())
    {
    QString str=file.readLine();
    ui->CodeDisplay->append(str);
    QStringList TokenList=str.split(' ',QString::SkipEmptyParts);
    if(TokenList[0].toInt())
    {
     int lineNum=TokenList[0].toInt();
     pro->insert(lineNum,str);
     displayCode(pro);
    }
else continue;
    }
}
void MainWindow::RunCode()//点击运行按钮是调用的信号，运行pro
{
    RunPro(pro->head->next);
}
void MainWindow::RunPro( statement *current)
{
    statement* curStmt=current;
    QStringList tmp;
    while(curStmt!=nullptr)
    {
      tmp=curStmt->text.split(' ',QString::SkipEmptyParts);
      //qDebug()<<"tmp[1] is:"<<tmp[1];
      tmp[1]=tmp[1].trimmed();
       if (tmp[1] == "END")
       {
           ui->treeDisplay->append(curStmt->getText());
           break;
       }
       else if (tmp[1] == "REM")
       {
           ui->treeDisplay->append(curStmt->getText());
           curStmt=curStmt->next;
           continue;
       }
       else if (tmp[1] == "LET")
       {

           letStmt(tmp,curStmt);
           curStmt=curStmt->next;
           continue;
       }
      else if (tmp[1] == "PRINT")
       {
         printStmt(tmp,curStmt);
         curStmt=curStmt->next;
         continue;
       }
      else if (tmp[1] == "INPUT")
       {
         QString variable=tmp[2].trimmed();
         QString str="? "+variable+" ";
         ui->cmdLineEdit->setText(str);
         QString dis=curStmt->getText();
         ui->treeDisplay->append(dis);
         RunAt=curStmt->next;
        break;
       }
      else if (tmp[1] == "IF")
       {
         curStmt=ifStmt(tmp,curStmt);
         continue;
       }
      else if (tmp[1] == "GOTO")
       {
         curStmt=GotoStmt(tmp,curStmt);
         continue;
     }
     else
     {
         QString error=tmp[0]+" Error";
         ui->treeDisplay->append(error);
          curStmt=curStmt->next;
     }

    }
}
bool MainWindow::IFcondition(QString str,QString &dis)
{
    QStringList IfStr;
    QString ch;
    if(str.contains("=")) ch="=";
    if(str.contains(">")) ch=">";
    if(str.contains("<")) ch="<";
    if(str.contains("<=")) ch="<=";
     if(str.contains(">=")) ch=">=";
    dis+=ch+"\n";
         IfStr=str.split(ch,Qt::SkipEmptyParts);
        QString ifLeft=IfStr[0];
        dis+="\t"+ifLeft+"\n";
        QStringList LeftExp=Tokenizer->ToTokens(ifLeft);
        Expression* val1;
        try {
            val1=Parser->ToTree(LeftExp);
        } catch (QString error) {
            QString err=error;
            throw err;
        }
        int leftValue=0;
        try {
            leftValue=val1->eval(Vars);
        } catch (QString error) {
           QString err=error;
           throw err;
        }

        QString ifRight=IfStr[1];
         dis+="\t"+ifRight+"\n";
        QStringList RightExp=Tokenizer->ToTokens(ifRight);
        Expression* val2;
        try {
            val2=Parser->ToTree(RightExp);
        } catch (QString error) {
            QString err=error;
            throw err;
        }

        int rightValue=0;
        try {
            rightValue=val2->eval(Vars);
        } catch (QString error) {
           QString err=error;
           throw err;
        }
        if(str.contains("=")) return leftValue==rightValue;
        else if(str.contains(">")) return leftValue>rightValue;
        else if(str.contains("<")) return leftValue<rightValue;
        else if(str.contains("<=")) return leftValue<=rightValue;
        else if(str.contains(">=")) return leftValue>=rightValue;
        else
        {
            ui->treeDisplay->append("Error! illegal operator!");
            return false;
        }
}
QString MainWindow::displayTree(Expression* Tree,int num )//num是进入递归的次数
{
    QString str="";
 if(Tree->type()==CONSTANT)
 {
     for(int i=0;i<num;++i)
     {
     str+="\t";
     }
     str+=QString::number(Tree->getValue())+"\n";
     return str;
 }
 else if(Tree->type()==IDENTIFIER)
 {
     for(int i=0;i<num;++i)
     {
     str+="\t";
     }
     str+=Tree->getIdentifierName()+"\n";
     return str;
 }
 else if(Tree->type()==COMPOUND)
 {
     for(int i=0;i<num;++i)
     {
     str+="\t";
     }
    str+=Tree->getOp()+"\n";
    str+=displayTree(Tree->getLHS(),++num);
    str+=displayTree(Tree->getRHS(),num);
    return str;
 }
 else return nullptr;
}
void MainWindow::letStmt(QStringList tmp,statement *curStmt)
{
     Expression* val;
     try {
       val=curStmt->buildTree(tmp,Tokenizer,Parser);
     } catch (QString error) {
         QString err=tmp[0]+" "+error;
         ui->treeDisplay->append(err);
         return;
     }

    try {
         val->eval(Vars);
    } catch (QString error) {
         QString err=tmp[0]+" "+error;
         ui->treeDisplay->append(err);
         return;
    }                        //计算表达式
    QString str="";
    int num=0;               //记录进入递归的次数
    str=str+tmp[0]+" "+tmp[1]+" "+displayTree(val,num);
    ui->treeDisplay->append(str);
}
statement* MainWindow::ifStmt(QStringList tmp, statement *curStmt)
{
    int j=0;
    for(j=0;j<tmp.length();++j)//找到Then子句
    {
        if(tmp[j]=="THEN") break;
    }
    if (j == tmp.length())
    {
        QString err=tmp[0]+" "+"Error";
        ui->treeDisplay->append(err);
        return curStmt->next;           //虽然是错的，但是生活还是要继续下去！！
    }
    int dst=tmp[j+1].toInt();//找到跳转语句的位置
    QString str="";
    QString dis=tmp[0]+" IF "+"THEN"+"\n"+" EXP";
    for(int i=2;i<tmp.length();++i)//找到Then前面的表达式
    {
        if(tmp[i]=="THEN") break;
        else str+=tmp[i];
    }

    bool condition;
    try {
         condition=IFcondition(str,dis);//判断条件是否成立
    } catch (QString error) {
        QString err=tmp[0]+" "+"Error";
        ui->treeDisplay->append(err);
        return curStmt->next;           //虽然是错的，但是生活还是要继续下去！！
    }

    dis+="\t"+tmp[j+1];
    ui->treeDisplay->append(dis);
    if(condition)            //如果成立,去找对应的成立的语句
    {
        statement* stmt=pro->head->next;
        while(stmt!=nullptr)//遍历搜索目的语句
            {
            QList<QString> stmtStr=stmt->text.split(' ',Qt::SkipEmptyParts);
                    if(stmtStr[0].toInt()==dst) break;
                    stmt=stmt->next;
            }
        return stmt;

    }
    else                //如果不成立,继续执行下一句
    {
        return curStmt->next;
    }
}
void MainWindow::printStmt(QStringList tmp, statement *curStmt)
{
    Expression* val;
    try {
      val=curStmt->buildTree(tmp,Tokenizer,Parser);
    } catch (QString error) {
        QString err=tmp[0]+" "+"Error";
        ui->treeDisplay->append(err);
        return;
    }
    int value=0;
    try {
         value=val->eval(Vars);
    } catch (QString error) {
        QString err=tmp[0]+" "+"Error";
        ui->treeDisplay->append(err);
        return;
    }
     QString num=QString::number(value);
     QString str="";
     int count=0;
     str=str+tmp[0]+" "+tmp[1]+" "+displayTree(val,count);
     ui->treeDisplay->append(str);
     ui->textBrowser->append(num);
}
statement* MainWindow::GotoStmt(QStringList tmp,statement* curStmt)
{
    int dst=tmp[2].toInt();//找到跳转语句的位置
    QString dis=tmp[0]+" "+tmp[1]+"\n"+"\t"+tmp[2];

    statement* stmt=pro->head->next;
    while(stmt!=nullptr)//遍历搜索目的语句
        {
        QList<QString> stmtStr=stmt->text.split(' ',Qt::SkipEmptyParts);
                if(stmtStr[0].toInt()==dst) break;
                stmt=stmt->next;
        }

    ui->treeDisplay->append(dis);
    if(stmt==nullptr)
    {
        QString error="The sentence is not existed!";
        ui->treeDisplay->append(error);
        return curStmt->next;           //挺住，我们看下一个语句

    }
     return stmt;

}
