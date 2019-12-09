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
   this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);


    box = new Textbox(this);
    con = new Console(this);
    edi = new Editor();
    clo=new QPushButton(this);
    min=new QPushButton(this);
    title_basic=new QLabel(this);

    title_basic->setGeometry(60,5,800,105);
    clo->setGeometry(920,0,80,40);
    min->setGeometry(840,0,80,40);
    box->setGeometry(60,120,880,580);
    con->setGeometry(60,760,880,43);

    title_basic->setText("BASIC_INTERPRET");
    title_basic->setScaledContents(true);
    title_basic->setStyleSheet("font-size:50px;font-family:Microsoft Yahei;border-radius:5px;border-width:5px");

    clo->setFlat(true);
    clo->setAutoFillBackground(true);
    QPalette palette=clo->palette();
    palette.setColor(QPalette::Button,QColor(255,0,0,100));
    clo->setPalette(palette);

    min->setFlat(true);
    min->setAutoFillBackground(true);
    palette=min->palette();
    palette.setColor(QPalette::Button,QColor(125,125,125,100));
    min->setPalette(palette);

    connect(clo,SIGNAL(clicked()),this,SLOT(close()));
    connect(min,SIGNAL(clicked()),this,SLOT(showMinimized()));
    connect(con,SIGNAL(newLineWritten(QString)),edi,SLOT(getlines(QString)));
    connect(edi,SIGNAL(print(QString)),box,SLOT(receivesentence(QString)));
    connect(edi,SIGNAL(perror(QString)),box,SLOT(receivesentence(QString)));
    connect(edi,SIGNAL(color(bool)),box,SLOT(change_color(bool)));
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

void Textbox::change_color(bool b)
{
    this->changecolor=b;
}

void Textbox::cleard()
{
   this->clear();
   this->write("Minimal BASIC --Type HELP for help");
}

//让每一行都拥有颜色
void Textbox::StringToHtmlFilter(QString &str)
{
       str.replace("&","&amp;");
       str.replace(">","&gt;");
       str.replace("<","&lt;");
       str.replace("\"","&quot;");
       str.replace("\'","&#39;");
       str.replace(" ","&nbsp;");
       str.replace("\n","<br>");
       str.replace("\r","<br>");
}

void Textbox::StringToHtml(QString &str, QColor crl)
{
        QByteArray array;
         array.append(crl.red());
         array.append(crl.green());
         array.append(crl.blue());
         QString strC(array.toHex());
         str = QString("<span style=\" color:#%1;\">%2</span>").arg(strC).arg(str);
}

void Textbox::receivesentence(QString str)
{
    if(this->changecolor)
    {
    QColor crlR(0,153,204);
    StringToHtmlFilter(str);
    StringToHtml(str,crlR);
    }
    this->changecolor=true;
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
