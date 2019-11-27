#ifndef PARSER_H
#define PARSER_H
#include <QStack>
#include <QQueue>
#include "exp.h"
#include "statement.h"
#include "evalstate.h"
class exp
{
public:
    Expression *ex;
    exp *Left;
    exp *Right;
};



class Parser
{
public:
    Parser();
    ~Parser();
    void Build_Tree(token *hd); //普通的赋值表达式树
    QString pre;
    void preread();
    void postread();
    exp* returnexp();
    int Calculate(EvaluationContext *context,exp *hd);

private:
    void preread(exp *tmp);
    void postread(exp *tmp);
    QStack<QString> operators;
    QStack<exp*> operands;
    QStack<Expression*> calculator;
    QQueue<Expression*> helper;
    exp *head;
};
#endif // PARSER_H
