#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <QObject>
#include <QWidget>
#include <statement.h>

class tokenizer : public QWidget
{
    Q_OBJECT
public:
    tokenizer();
    ~tokenizer();

    void clear();
    void IntoToken();
    void showtokens();

    void Statetype();
    void RUN();

    statementtype St;

    int Current_Line_index();
private:
   token *head,*currenttoken,*rear;

   QString line;

   statement *stm;
   EvaluationContext *eva;
signals:
   void print(QString l);
   void GOTO_Line(int n);
public slots:
   void getline(const QString l);
   void getContext(EvaluationContext *eva);
};

#endif // TOKENIZER_H
