#ifndef BASIC_H
#define BASIC_H

#include <QMainWindow>
#include <QTextEdit>
#include <QObject>
#include <QTextBrowser>

#include "program.h"
class Textbox;
class Console;

class Basic : public QMainWindow
{
    Q_OBJECT

public slots:
    void Quit();
public:
    Basic(QWidget *parent = 0);
    ~Basic();
private:
    static const int BASIC_WINDOW_WIDTH = 1000;
    static const int BASIC_WINDOW_HEIGHT = 800;
    Textbox *box;
    Console *con;
    Editor  *edi;
};


class Textbox : public QTextBrowser
{
    Q_OBJECT

public:
    explicit Textbox(QWidget *parent = nullptr);
    void write(QString str);
signals:

public slots:
    void receivesentence(QString str);
    void cleard();
};

class Console : public QTextEdit
{
    Q_OBJECT
public:
    explicit Console(QWidget *parent = nullptr);

signals:
    void newLineWritten(QString newline);

public slots:
    void cleard();
    void write(QString msg);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};
#endif // BASIC_H
