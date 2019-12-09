#ifndef BASIC_H
#define BASIC_H

#include <QMainWindow>
#include <QTextEdit>
#include <QObject>
#include <QTextBrowser>
#include <QColor>
#include <QPushButton>
#include <QLabel>

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
    static const int BASIC_WINDOW_HEIGHT = 870;
    QPushButton *clo;
    QPushButton *min;
    QLabel *title_basic;
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
    void StringToHtmlFilter(QString &str);
    void StringToHtml(QString &str,QColor crl);
signals:

public slots:
    void receivesentence(QString str);
    void cleard();
    void change_color(bool);
private:
    bool changecolor=true;
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
