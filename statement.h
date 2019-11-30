#ifndef STATEMENT_H
#define STATEMENT_H
#include <QString>
#include "evalstate.h"
class exp;


class statement;

class Parser;

enum statementtype{REM, LET, PRINT, INPUT, GOTO, IF, END };

class token{
 public:
    QString tokens;
    token *next;

};
//每个类加个信号以发功能？
class statement{
public:
    statement();
    virtual ~statement();
    virtual void gettoken(const token *hd);
    virtual void getContext(EvaluationContext *con);
    virtual statementtype type()=0;
    virtual int Line()=0;

    virtual bool have_THEN();
    virtual int IF_Judge_Condition();

    virtual QString parseEXP();
    virtual void CalculateEXP();
    virtual void Calculate_IF_EXP();
    virtual void parseIF_EXP();

    virtual bool returnIF_bool();
    virtual QString returnINPUT();
    virtual QString returnIF();
    virtual int returnGOTO();
    virtual int returnPRINT();

private:

};

class REMstatement:public statement{
public:
    REMstatement(int line);
    virtual ~REMstatement();
    virtual void gettoken(const token *hd);
    virtual statementtype type();
    virtual int Line();

private:
    int linenumber;
    token *head,*currenttok,*rear;
};

class LETstatement:public statement{ //要将表达式的值存入qmap
public:
    LETstatement(int line);
    virtual ~LETstatement();
    virtual void gettoken(const token *hd);
    virtual statementtype type();
    virtual int Line();
    virtual QString parseEXP();
    virtual void CalculateEXP();
    virtual void getContext(EvaluationContext *con);

private:
    int linenumber;
    token *head,*currenttok,*rear;   //head不存token，之后存着表达式的token：就比如a=b+c之类的
    QString preOrder;

    exp *head_LET;
    EvaluationContext *context;
};

class PRINTstatement:public statement{ //PRINT后应当只有一个表达式
public:
    PRINTstatement(int line);
    virtual ~PRINTstatement();
    virtual void gettoken(const token *hd);
    virtual statementtype type();
    virtual int Line();
    virtual QString parseEXP();
    virtual void CalculateEXP();
    virtual void getContext(EvaluationContext *con);
    virtual int returnPRINT();

private:
    int linenumber;
    token *head,*currenttok,*rear;
    QString preOrder;

    exp *head_PRINT;
    EvaluationContext *context;

    int PRINT_Number;
};

class INPUTstatement:public statement{
public:
    INPUTstatement(int line);
    virtual ~INPUTstatement();
    virtual statementtype type();
    virtual int Line();
    virtual void gettoken(const token *hd);
    virtual QString returnINPUT(){return INPU;};
    virtual void getContext(EvaluationContext *con);

private:
    int linenumber;
    QString INPU;

    EvaluationContext *context;
};

class GOTOstatement:public statement{    //GOTO后面可能是个表达式
public:
    GOTOstatement(int line);
    virtual ~GOTOstatement();
    virtual statementtype type();
    virtual int Line();
    virtual int returnGOTO();
    virtual void gettoken(const token *hd);

private:
    int linenumber;
    int gotoline;
};

class IFstatement:public statement{
public:
    IFstatement(int line);
    virtual ~IFstatement();
    virtual statementtype type();
    virtual int Line();
    virtual void gettoken(const token *hd);

    virtual bool have_THEN();
    virtual bool returnIF_bool();

    virtual QString parseEXP();
    virtual void CalculateEXP();
    virtual void Calculate_IF_EXP();
    virtual void parseIF_EXP();

    virtual QString returnIF();//返回THEN之后的第一个tok
    virtual int returnGOTO();

    virtual void getContext(EvaluationContext *con);
    virtual int IF_Judge_Condition();//1代表=，2代表>,3代表<，4代表>=，5代表<=,0代表不成功。

private:
    int linenumber;
    token *head1,*currenttok1,*rear1; //一句存IF后的
    token *head1_1,*currenttok1_1,*rear1_1;
    token *head1_2,*currenttok1_2,*rear1_2;
    token *head2,*currenttok2,*rear2; //一句存THEN后的,如果没THEN则head2->next=rear2（只有THEN没有语句的情况也是）

    QString preOrder1_1;
    QString preOrder1_2;
    QString preOrder;

    exp *head1_IF;
    exp *head2_IF;
    exp *head3_IF;

    EvaluationContext *con;

    int t1_1;
    int t1_2;
    int t;
};

class ENDstatement:public statement{
public:
    ENDstatement(int line);
    virtual ~ENDstatement();
    virtual statementtype type();
    virtual int Line();
    virtual void gettoken(const token *hd);

private:
    int linenumber;

};

#endif // STATEMENT_H
