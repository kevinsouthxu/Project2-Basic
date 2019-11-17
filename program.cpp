#include "program.h"
#include <limits>
#include <QFile>
#include <QWidget>
ListBuffer::ListBuffer()
{
    this->clear();
}
void ListBuffer::clear()
{
   head=new Listrec;
   rear=new Listrec;
   currentline=new Listrec;
   head->number=-1;
   head->next=rear;
   rear->number=std::numeric_limits<int>::max();
}
void ListBuffer::deleteLine(int line_idx)
{
    Listrec *tmp=new Listrec;
    Listrec *p=new Listrec;
    tmp=head->next;
    p=head;
    while(tmp!=rear)
    {
        if(tmp->number==line_idx)
         {p->next=p->next->next;
          break;
         }
       tmp=tmp->next;
       p=p->next;
    }
}

void ListBuffer::showLines() const
{
    Listrec *tmp=new Listrec;
    tmp=head->next;
    emit flush();
    while(tmp!=rear)
    {
      emit print(tmp->list);
      tmp=tmp->next;
    }
}

void ListBuffer::insertLine(int line_idx, const QString &text)
{
   Listrec *tmp=new Listrec;
   tmp->number=line_idx;
   tmp->list=text;
   if(line_idx<0)
      throw "cannot enter the number that <0";

   currentline=head;
   while(currentline!=rear)
   {
     if((tmp->number>currentline->number)&&(tmp->number<currentline->next->number))
     {
         tmp->next=currentline->next;
         currentline->next=tmp;
         break;
     }
     else if(tmp->number==currentline->number)
     {
         Listrec *p=new Listrec;
         p=head;
         while(p->next!=currentline)
          p=p->next;
         tmp->next=currentline->next;
         p->next=tmp;
         delete currentline;
         break;
     }
     currentline=currentline->next;
   }
}

void ListBuffer::writeToFile(const QString &filename) const
{
    QFile qfile(filename);
    if(!qfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        emit perror("cannot open file.");
    }
    Listrec *p=new Listrec;
    p=head->next;
    while(p!=rear)
    {
        qfile.write(p->list.toUtf8());
        qfile.write("\n");
        p=p->next;
    }
    qfile.close();
}

void ListBuffer::printtok(QString l)
{
    emit print(l);
}
void ListBuffer::runmode()
{
    Eva=new EvaluationContext;
    Listrec *tmp=new Listrec;
    tmp=head->next;
    while(tmp!=rear)
    {
        tmp->tok= new tokenizer;
        tmp->tok->getline(tmp->list);
        connect(tmp->tok,SIGNAL(print(QString)),this,SLOT(printtok(QString)));
        //tmp->tok->showtokens();
        tmp->tok->getContext(Eva);
        tmp->tok->Statetype();

      tmp=tmp->next;
    }

    tmp=head->next;
    while(tmp!=rear&&tmp->tok->St!=END)
    {
     tmp->tok->RUN();
     tmp=tmp->next;
    }
}
ListBuffer::~ListBuffer()
{
}
/**/
Editor::Editor()
{
    buffer= new ListBuffer;
    connect(buffer,SIGNAL(print(QString)),this,SLOT(printline(QString)));
    connect(buffer,SIGNAL(perror(QString)),this,SLOT(perrorline(QString)));
    connect(buffer,SIGNAL(flush()),this,SLOT(emitflush()));
}
void Editor::cmdwrite(const QString &filename)
{
    buffer->writeToFile(filename);
}
void Editor::cmddelete(int number)
{
   buffer->deleteLine(number);
}
void Editor::cmdshow()
{
    buffer->showLines();
}
void Editor::cmdinsert(int number,const QString &content)
{
    buffer->insertLine(number,content);
}
void Editor::cmdclear()
{
    buffer->clear();
}

void Editor::run()
{
    QString cmd;
        cmd=line.trimmed();
        if (cmd == "Q")
            return;
            dispatchCmd(cmd);


}

void Editor::HELPcommand()
{
    QFile qfile("../BasicInterpret/HELP.txt");
    if(!qfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit perror("cannot open file.");
        return;
    }
    QByteArray line=qfile.readAll();
    emit print(QString(line));
    qfile.close();
}

void Editor::dispatchCmd(const QString &cmd)
{
    if(cmd[0] =='d')                       //delete
    {
        QChar *ch=new QChar[cmd.length()-1] ;
        for(int i=0;i<cmd.length()-2;++i)
        ch[i]=cmd[i+2];
        int p=0;
        for(int i=0;i<cmd.length()-2;++i)
        if(ch[i]>='0'&&ch[i]<='9')
        p=int(ch[i].toLatin1()-'0')+p*10;
        delete []ch;
        cmddelete(p);
        cmdshow();
        return;
    }
    if(cmd=="RUN")
    {
        this->buffer->runmode();
        return;
    }
    if(cmd=="SAVE")   //print
   {
       cmdshow();
       return;
   }
   if(cmd[0]=='S'&&cmd[1]=='A'&&cmd[2]=='V'&&cmd[3]=='E')
   {
    QString name=cmd.mid(5,cmd.length()-5);
    cmdwrite(name);
    return;
   }
   if(cmd=="CLEAR")
   {
     cmdclear();
     cmdshow();
     return;
   }
   if(cmd=="QUIT")
   {
       emit QUIT();
       return;
   }
   if(cmd=="HELP")
   {
     HELPcommand();
     return;
   }
   int i=0;
   int sum=-1;
   while(cmd[i]>='0'&&cmd[i]<='9')
    {
       if(sum==-1) sum=0;
       sum=int(cmd[i].toLatin1()-'0')+sum*10;
       i++;
    }
    if(sum!=-1)                       //insert
      {
     cmdinsert(sum,cmd);
     cmdshow();
     return;
      }
   if(cmd[0]=='-' && cmd[1]>='0' && cmd[1]<='9')
       throw "The Line Index cannot be less than 0";
   throw "UNKNOWN COMMAND";

}

Editor::~Editor()
{
    delete buffer;
}
void Editor::getlines(QString cin){
    line=cin;
    run();
}
void Editor::printline(QString line) const{
    emit print(line);
}
void Editor::perrorline(QString line) const{
    emit perror(line);
}
void Editor::emitflush()
{
    emit flush();
}
