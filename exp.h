
#ifndef EXP_H
#define EXP_H
# include <string>
# include <map>
# include "tokenizer.h"
# include "evalstate.h"
class EvaluationContext;
enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };
class Expression {
public:

   Expression();

   virtual ~Expression();
   virtual int eval(evalstate &Vars) = 0;
   virtual ExpressionType type() = 0;
   virtual QString getIdentifierName() = 0;
   virtual int getValue()=0;
   virtual QString getOp()=0;
   virtual  Expression *getLHS()=0 ;
   virtual  Expression *getRHS()=0;
};

class ConstantExp: public Expression {
public:
   ConstantExp(int val);
   ~ConstantExp();
   int eval(evalstate &Vars) override;
   ExpressionType type() override;
   virtual QString getIdentifierName() override;
  int getValue() ;
  virtual QString getOp(){return "";};
  virtual  Expression *getLHS(){return nullptr;} ;
  virtual  Expression *getRHS(){return nullptr;};
private:
   int value;
};
class IdentifierExp: public Expression {
public:
   IdentifierExp(QString name);
   ~IdentifierExp();
   int eval(evalstate &Vars) override;
   ExpressionType type() override;
   QString getIdentifierName() override;
   virtual int getValue(){return 0;};
   virtual QString getOp(){return "";};
   virtual  Expression *getLHS(){return nullptr;} ;
   virtual  Expression *getRHS(){return nullptr;};
private:
   QString name;
};
class CompoundExp: public Expression {
public:
   CompoundExp(QString op, Expression *lhs, Expression *rhs);
   ~CompoundExp();
   int eval(evalstate &Vars) override;
   ExpressionType type() override;
   QString getOperator() ;
   Expression *getLHS() ;
   Expression *getRHS() ;
   virtual QString getIdentifierName() override;
   QString getOp();
   virtual int getValue(){return 0;};

private:
   QString op;
   Expression *lhs, *rhs;
};
#endif








