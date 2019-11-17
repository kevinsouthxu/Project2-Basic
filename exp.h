#ifndef EXP_H
#define EXP_H
#include<string>
#include<QString>
#include <QObject>
#include <QMap>
#include "evalstate.h"
/**/
class EvaluationContext;

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND};

class Expression{

public:

   Expression();
   virtual ~Expression();
   virtual int eval(EvaluationContext & context) = 0;
   virtual QString toString() = 0;
   virtual ExpressionType type() = 0;

/* getter methods for convenience */
   virtual int getConstantValue();
   virtual QString getIdentiferName();
   virtual QString getOperator();
   virtual Expression *getLHS();
   virtual Expression *getRHS();

};

class ConstantExp: public Expression{

public:

    ConstantExp(int val);

    virtual int eval(EvaluationContext & context);
    virtual QString toString();
    virtual ExpressionType type();

    virtual int getConstantValue();

private:

    int value;
};

class IdentifierExp: public Expression {

public:

    IdentifierExp(QString name);

    virtual int eval(EvaluationContext & context);
    virtual QString toString();
    virtual ExpressionType type();

    virtual QString getIdentifierName();

private:

    QString name;
};

class CompoundExp: public Expression
{
public:
    CompoundExp(QString op, Expression *lhs=nullptr, Expression *rhs=nullptr);
    virtual ~CompoundExp();

    virtual int eval(EvaluationContext & context);
    virtual QString toString();
    virtual ExpressionType type();

    virtual QString getOperator();
    virtual Expression  *getLHS();
    virtual Expression  *getRHS();

private:

    QString op;
    Expression *lhs, *rhs;
};

#endif // EXP_H
