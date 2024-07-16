#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"evalstate.h"
#include"statement.h"
#include"tokenizer.h"
#include"parser.h"
#include"program.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    tokenizer *Tokenizer;
    parser* Parser;
    evalstate Vars;
    statement* directCode; //操作直接输入没有行号的代码

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void RunPro(statement *current);
    QString displayTree(Expression* Tree,int num);
    bool IFcondition(QString str,QString &dis);
    void displayCode(program* pro);

private slots:

    void letStmt(QStringList tmp,statement* curStmt);
    statement* ifStmt(QStringList tmp,statement* curStmt);
    void printStmt(QStringList tmp,statement* curStmt);
    statement* GotoStmt(QStringList tmp,statement* curStmt);
    void on_cmdLineEdit_editingFinished();
    void clearAll();
    void loadCode();
    void RunCode();

private:
    program *pro;
    statement* RunAt=nullptr;//记录当前代码的位置
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
