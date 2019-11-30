#include "basic.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QKeyEvent>
#include <QTextLine>
#include <QTextCursor>
Basic::Basic(QWidget *parent)
    : QMainWindow (parent)
{
    this->setWindowTitle("Basic");
    this->setFixedSize(BASIC_WINDOW_WIDTH, BASIC_WINDOW_HEIGHT);

    box = new Textbox(this);
    con = new Console(this);
    edi = new Editor();
    box->setGeometry(60,60,880,580);
    con->setGeometry(60,700,880,43);

    connect(con,SIGNAL(newLineWritten(QString)),edi,SLOT(getlines(QString)));
    connect(edi,SIGNAL(print(QString)),box,SLOT(receivesentence(QString)));
    connect(edi,SIGNAL(perror(QString)),box,SLOT(receivesentence(QString)));
    connect(edi,SIGNAL(flush()),box,SLOT(cleard()));
    connect(edi,SIGNAL(QUIT()),this,SLOT(Quit()));
}

Basic::~Basic()
{
}
void Basic::Quit()
{
    this->close();
}
Textbox::Textbox(QWidget *parent) : QTextBrowser (parent)
{
   this->cleard();
}

void Textbox::write(QString sentence)
{
   this->append(sentence);
}

void Textbox::cleard()
{
   this->clear();
   this->write("Minimal BASIC --Type HELP for help");
}

void Textbox::receivesentence(QString str)
{
  this->write(str);
}

Console::Console(QWidget *parent) : QTextEdit(parent)
{
    this->setPlaceholderText("enter sentence!");
}

void Console::cleard()
{
    this->clear();
}

void Console::write(QString msg)
{
    this->append(msg);
}

void Console::keyPressEvent(QKeyEvent *event)
{
    if (this->textCursor().hasSelection())
        return;
    if (event->key() == Qt::Key_Return) {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        QString lastLine = cursor.selectedText();
        this->cleard();
        newLineWritten(lastLine);
    }
    QTextEdit::keyPressEvent(event);
}
