#ifndef PROGRAM_H
#define PROGRAM_H
#include <string>
#include <QString>
#include <QObject>
#include <QWidget>

#include "tokenizer.h"
#include "evalstate.h"
class ListBuffer: public QWidget
{
       Q_OBJECT
private:
    class Listrec{
        public:
        QString list;
        int number=0;
        Listrec *next;
        tokenizer *tok;
    };
    Listrec *head,*currentline,*rear;
    EvaluationContext *Eva;
public slots:
    void printtok(QString);
signals:
    void perror(QString) const;
    void print(QString) const;
    void flush() const;
public:
    ListBuffer();
    ~ListBuffer();

    void writeToFile(const QString &filename) const;
    void clear();
    void showLines() const;
    void deleteLine(int line_idx);
    void insertLine(int line_idx, const QString &text);
    void runmode();
};

class Editor:public QWidget
{
    Q_OBJECT
signals:
  void perror(QString) const;
  void print(QString) const;
  void getlines();
  void flush();

  void QUIT();
private:
  ListBuffer *buffer;

  void dispatchCmd(const QString &cmd);
  void cmdwrite(const QString &filename);
  void cmddelete(int number);
  void cmdinsert(int number,const QString &content);
  void cmdshow();
  void cmdclear();

  QString line;
public:
  Editor();
  ~Editor();

  void run();
  void HELPcommand();
public slots:
  void getlines(QString cin);
  void printline(QString line) const;
  void perrorline(QString line) const;
  void emitflush();
};
#endif // PROGRAM_H
