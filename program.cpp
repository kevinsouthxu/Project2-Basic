#include "program.h"
#include "text_error.h"
#include <limits>
#include <QFile>
#include <QWidget>
#include <QThread>
#include <QCoreApplication>
ListBuffer::ListBuffer()
{
    this->clear();
    Create_new_eva=false;
}
void ListBuffer::clear()
{
   head=new Listrec;
   rear=new Listrec;
   currentline=new Listrec;
   Eva=new EvaluationContext;
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
void ListBuffer::get_line_index(int i)
{
    this->Current_Line_index=i;
    int temp=currentline->tok->Current_Line_index();
    if(temp==this->Current_Line_index)
    {
        print("Line:"+QString::number(temp)+"\t"+"CANT address this address.");
        CAN_CONTINUE_RUN=false;
        return;
    }
    currentline=head->next;
    Listrec *temp_node=new Listrec;
    temp_node=head;
    while(currentline!=rear)
    {
        if(Current_Line_index==currentline->tok->Current_Line_index())
            break;
        currentline=currentline->next;
        temp_node=temp_node->next;
    }
    if(currentline==rear)
    {
         print("Line:"+QString::number(temp)+"\t"+"There is no Linenumber behind the GOTO command.");
         CAN_CONTINUE_RUN=false;
    }
    currentline=temp_node;
    return;
}
void ListBuffer::get_input_signal(int)
{
    if(CAN_CONTINUE_RUN)
    {
    print("?");
    this->PAUSE=true;
    }
}
void ListBuffer::error_situation()
{
    this->CAN_CONTINUE_RUN=false;
}
void ListBuffer::get_input_value(QString cin)
{
    bool ok;
    int value=cin.toInt(&ok);
    int temp=0;
    temp=currentline->tok->Current_Line_index();
    if(!ok)
    {
        print("Line:"+QString::number(temp)+"\t"+"There is UNVALID input.");
        CAN_CONTINUE_RUN=false;
    }
    this->Eva->setValue(currentline->tok->stm->returnINPUT(),value);
    this->PAUSE=false;
}
void ListBuffer::I_P_L(const QString &text)
{
    CAN_CONTINUE_RUN=true;
    Create_new_eva=true;
    Listrec *tmp=new Listrec;
    currentline=tmp;
    tmp->list=text;

    PAUSE=false;
    tmp->tok= new tokenizer;
    tmp->tok->getline(tmp->list);
    connect(tmp->tok,SIGNAL(print(QString)),this,SLOT(printtok(QString)));
    connect(tmp->tok,SIGNAL(GOTO_Line(int)),this,SLOT(get_line_index(int)));
    connect(tmp->tok,SIGNAL(INPUT_Line(int)),this,SLOT(get_input_signal(int)));
    connect(tmp->tok,SIGNAL(error()),this,SLOT(error_situation()));
    tmp->tok->getContext(Eva);
    if(CAN_CONTINUE_RUN)
       tmp->tok->Statetype();

    if(text.mid(2,5)=="INPUT")
    {
        tmp->tok->RUN();
            while(PAUSE&&CAN_CONTINUE_RUN)
            {
                if(!CAN_CONTINUE_RUN)
                    break;
                QCoreApplication::processEvents();
                QThread::msleep(20);
            }
    }
    else
    {
        if(CAN_CONTINUE_RUN)
             tmp->tok->RUN();
    }

    disconnect(tmp->tok,SIGNAL(print(QString)),this,SLOT(printtok(QString)));
    disconnect(tmp->tok,SIGNAL(GOTO_Line(int)),this,SLOT(get_line_index(int)));
    disconnect(tmp->tok,SIGNAL(INPUT_Line(int)),this,SLOT(get_input_signal(int)));
    disconnect(tmp->tok,SIGNAL(error()),this,SLOT(error_situation()));

    delete tmp;
}
void ListBuffer::runmode()
{
    if(!Create_new_eva)
      Eva=new EvaluationContext;
    Create_new_eva=false;
    Listrec *tmp=new Listrec;
    tmp=head->next;
    CAN_CONTINUE_RUN=true;
    this->showLines();
    while(tmp!=rear)
    {
        tmp->tok= new tokenizer;
        tmp->tok->getline(tmp->list);
        connect(tmp->tok,SIGNAL(print(QString)),this,SLOT(printtok(QString)));
        connect(tmp->tok,SIGNAL(GOTO_Line(int)),this,SLOT(get_line_index(int)));
        connect(tmp->tok,SIGNAL(INPUT_Line(int)),this,SLOT(get_input_signal(int)));
        connect(tmp->tok,SIGNAL(error()),this,SLOT(error_situation()));
        tmp->tok->getContext(Eva);
        if(CAN_CONTINUE_RUN)
           tmp->tok->Statetype();

        tmp=tmp->next;
    }

    currentline=head->next;
    PAUSE=false;

    while(currentline!=rear&&currentline->tok->St!=END&&CAN_CONTINUE_RUN)
    {
        Current_Line_index=currentline->tok->Current_Line_index();
        currentline->tok->RUN();

        if(!CAN_CONTINUE_RUN)
            break;
        while(PAUSE)
        {
            QCoreApplication::processEvents();
            QThread::msleep(20);
        }
        currentline=currentline->next;
    }
    currentline=head->next;
    while(currentline!=rear)
    {
        disconnect(currentline->tok,SIGNAL(print(QString)),this,SLOT(printtok(QString)));
        disconnect(currentline->tok,SIGNAL(GOTO_Line(int )),this,SLOT(get_line_index(int)));
        disconnect(currentline->tok,SIGNAL(INPUT_Line(int)),this,SLOT(get_input_signal(int)));
        disconnect(currentline->tok,SIGNAL(error()),this,SLOT(error_situation()));
        currentline=currentline->next;
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
    line_number.clear();
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
    //line_number.clear();
    buffer->clear();
}
void Editor::cmdI_P_L(const QString &cmd)
{
    buffer->I_P_L(cmd);
}

void Editor::run()
{
    QString cmd;
        cmd=line.trimmed();//对每一行简化处理
        if (cmd == "Q")
            return;
        try {
             dispatchCmd(cmd);
        } catch (text_error error) {
            this->cmdshow();
            print(error.get_error_information());
        }


}

void Editor::HELPcommand()
{
    QFile qfile("../BasicInterpret/HELP.txt");
    if(!qfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit print("cannot open file.");
        return;
    }
    QByteArray line=qfile.readAll();
    emit color(false);
    emit print(QString(line));
    qfile.close();
}

void Editor::dispatchCmd( QString &cmd)
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
   if((cmd.length()>=5&&cmd.left(5)=="INPUT")||(cmd.length()>=5&&cmd.left(5)=="PRINT")||(cmd.length()>=3&&cmd.left(3)=="LET"))
   {
      cmd.insert(0,QString("0 "));
       cmdI_P_L(cmd);
      return;
   }
   int i=0;
   int sum=-1;
   while(i<cmd.length()&&cmd.at(i)>='0'&&cmd.at(i)<='9')
    {
       if(sum==-1) sum=0;
       sum=int(cmd[i].toLatin1()-'0')+sum*10;
       i++;
    }
    if(sum!=-1&&sum!=0)                       //insert
      {
      //  line_number.push(sum);
     cmdinsert(sum,cmd);
     cmdshow();
     return;
      }
   if(cmd[0]=='-' && cmd[1]>='0' && cmd[1]<='9')
   {
       text_error error("The Line Index cannot be less than 0");
       throw error;
   }
   text_error error("UNKNOWN COMMAND");
   throw error;

}

Editor::~Editor()
{
    delete buffer;
}
void Editor::getlines(QString cin){
    if(!this->buffer->PAUSE)
    {line=cin;
     run();
    }
    else {
        this->buffer->get_input_value(cin);
    }
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
