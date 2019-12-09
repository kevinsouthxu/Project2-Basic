#include "tokenizer.h"
#include "parser.h"
#include <QChar>
#include "text_error.h"
tokenizer::tokenizer()
{
    this->clear();
}

tokenizer::~tokenizer()
{
    delete head;
    delete currenttoken;
    delete rear;
}

void tokenizer::clear()
{
    head=new token;
    rear=new token;
    currenttoken=new token;
    head->next=rear;
    rear->tokens="\0";
}

void tokenizer::getline(const QString l)
{
    this->line=l;
    this->IntoToken();
    return;
}

void tokenizer::getContext(EvaluationContext *eva){
    this->eva=eva;
}

void tokenizer::showtokens()
{
    token *tmp= new token;
    tmp=head;
    while(tmp!=rear)
    {
        emit print(tmp->tokens);
        tmp=tmp->next;
    }
}

void tokenizer::IntoToken()
{
    currenttoken->next=rear;
    head->next=currenttoken;
    token *tmp;
    int i;
    int count=0;
    bool flag=true;//command or not
    line=line.simplified();
    while((this->line[count]>='0'&&this->line[count]<='9')||this->line[count]==' ')
    {
        if(this->line[count]!=' ')
          head->tokens.append(this->line[count]);
        count++;
    }
    for(i=count;i<line.length();i++)
    {
        if(flag)
          if(this->line[i]>='A'&&this->line[i]<='Z')
          {
                currenttoken->tokens.append(this->line[i]);
                continue;
          }
        flag=false;

        if((this->line[i]>='a'&&this->line[i]<='z')
                ||(this->line[i]>='A'&&this->line[i]<='Z')
                ||this->line[i]=='_'
                ||(this->line[i]>='0'&&this->line[i]<='9'
                )
                )
        {

            if(currenttoken->tokens.contains('=')
                ||currenttoken->tokens.contains('+')
                ||currenttoken->tokens.contains('-')
                ||currenttoken->tokens.contains('*')
                ||currenttoken->tokens.contains('/')
                ||currenttoken->tokens.contains('>')
                ||currenttoken->tokens.contains('<')
                ||currenttoken->tokens.contains('(')
                ||currenttoken->tokens.contains(')'))
               {
                if(!(currenttoken->tokens=="-"&&(this->line[i]>='0'&&this->line[i]<='9')&&(this->line[i-2]=='='||this->line[i-2]=='(')))
                {
                    tmp=new token;
                    tmp->next=currenttoken->next;
                    currenttoken->next=tmp;
                    currenttoken=currenttoken->next;
                }
               }
            currenttoken->tokens.append(this->line[i]);
            continue;
        }
        if(this->line[i]=='='
           ||this->line[i]=='+'
           ||this->line[i]=='-'
           ||this->line[i]=='*'
           ||this->line[i]=='/'
           ||this->line[i]=='>'
           ||this->line[i]=='<'
           ||this->line[i]=='('
           ||this->line[i]==')')
           {
                if(currenttoken->tokens.length()!=0)
                {
                   if(currenttoken->tokens!="*"||this->line[i]!='*')
                   {
                    tmp=new token;
                    tmp->next=currenttoken->next;
                    currenttoken->next=tmp;
                    currenttoken=currenttoken->next;
                   }

                }
                currenttoken->tokens.append(this->line[i]);
                continue;
        }
            tmp=new token;
            tmp->next=currenttoken->next;
            currenttoken->next=tmp;
            currenttoken=currenttoken->next;

    }
}

void tokenizer::Statetype()
{
     token *tmp=new token;
     tmp=head->next;   //此时tmp指向的是存着命令的地方
     if(tmp->tokens=="REM")
     {
        this->St=REM;
        this->stm=new REMstatement(head->tokens.toInt());
        this->stm->gettoken(tmp->next);
         return;
     }
     if(tmp->tokens=="LET")   //捕捉异常与表达式有关
     {
        this->St=LET;
        this->stm=new LETstatement(head->tokens.toInt());
        this->stm->gettoken(tmp->next);
         try {
             QString t(this->stm->parseEXP());
         } catch (text_error error) {
          print("Line: "+QString::number(this->stm->Line())+"\t"+error.get_error_information());
          emit this->error();
         }
         return;
     }
     if(tmp->tokens=="PRINT")  //捕捉异常与表达式有关
     {
        this->St=PRINT;
        this->stm=new PRINTstatement(head->tokens.toInt());
        this->stm->gettoken(tmp->next);
        QString t(this->stm->parseEXP());
         return;
     }
     if(tmp->tokens=="INPUT")
     {
        this->St=INPUT;
        this->stm=new INPUTstatement(head->tokens.toInt());
        try{
        this->stm->gettoken(tmp->next);
         }
        catch (text_error error)
         {
             print("Line: "+QString::number(this->stm->Line())+"\t"+error.get_error_information());
             emit this->error();
         }
         return;
     }
     if(tmp->tokens=="GOTO")
     {
        this->St=GOTO;
        this->stm=new GOTOstatement(head->tokens.toInt());
         try{
         this->stm->gettoken(tmp->next);
          }
         catch (text_error error)
          {
             print("Line: "+QString::number(this->stm->Line())+"\t"+error.get_error_information());
             emit this->error();
          }
         return;
     }
     if(tmp->tokens=="IF")
     {
        this->St=IF;
        this->stm=new IFstatement(head->tokens.toInt());
         try{
         this->stm->gettoken(tmp->next);
          }
         catch (text_error error)
          {
             print("Line: "+QString::number(this->stm->Line())+"\t"+error.get_error_information());
             emit this->error();
             return;
          }
         try {
             this->stm->parseIF_EXP();
         } catch (text_error error) {
             print("Line: "+QString::number(this->stm->Line())+"\t"+error.get_error_information());
             emit this->error();
             return;
         }
        QString t(this->stm->returnIF());
         if(stm->have_THEN())
         {
             try
             {
                 QString t1(this->stm->parseEXP());
             } catch (text_error error){
                 print("Line: "+QString::number(this->stm->Line())+"\t"+error.get_error_information());
                 emit this->error();
             }

         }

         return;
     }
     if(tmp->tokens=="END")
     {
        this->St=END;
        this->stm=new ENDstatement(head->tokens.toInt());
         try{
         this->stm->gettoken(tmp->next);
          }
         catch (text_error error)
          {
              print("Line: "+QString::number(this->stm->Line())+"\t"+error.get_error_information());
              emit this->error();
          }
         return;
     }
     text_error error("exist UNKNOWN statements.");
     print(error.get_error_information());
     emit this->error();
}

void tokenizer::RUN()
{
    token *tmp=new token;
    tmp=head->next;   //此时tmp指向的是存着命令的地方
    if(this->St==REM)
    {
        return;
    }
    if(this->St==LET)
    {
      this->stm->getContext(this->eva);
        try{
      this->stm->CalculateEXP();
        }
        catch (text_error error)
        {
            print("Line:"+QString::number(this->stm->Line())+"\t"+error.get_error_information());
            emit this->error();
            return;
        }
        return;

    }
    if(this->St==PRINT)
    {
      this->stm->getContext(this->eva);
        try{
      this->stm->CalculateEXP();
        }
        catch (text_error error)
        {
            print("Line:"+QString::number(this->stm->Line())+"\t"+error.get_error_information());
            emit this->error();
            return;
        }
      print(QString::number(this->stm->returnPRINT()));
      return;
    }
    if(this->St==INPUT)
    {
        this->stm->getContext(this->eva);
        emit INPUT_Line(1);
        return;
    }
    if(this->St==GOTO)
    {
      emit GOTO_Line(this->stm->returnGOTO());
        return;
    }
    if(this->St==IF)
    {
        this->stm->getContext(this->eva);
        bool whether=false;
        try {
            this->stm->CalculateEXP();
        } catch (text_error error) {
            print("Line:"+QString::number(this->stm->Line())+"\t"+error.get_error_information());
            emit this->error();
            return;
        }
        try {
            whether=this->stm->returnIF_bool();
        } catch (text_error error) {
            print("Line:"+QString::number(this->stm->Line())+"\t"+error.get_error_information());
            emit this->error();
            return;
        }
        if(whether)
        {
            QString THEN_COMMAND=this->stm->returnIF();
            bool ok;
            int temp=THEN_COMMAND.toInt(&ok);
            if(ok)
            {
                    emit GOTO_Line(temp);
                    return;
            }
            if(THEN_COMMAND=="GOTO")
            {emit GOTO_Line(this->stm->returnGOTO());
                return;
            }
            if(THEN_COMMAND=="LET")
            {
                print("Line:"+QString::number(this->stm->Line())+"\t"+
                      "You only allowed to write the expression directly after THEN");
                emit this->error();
                return;
            }
            try {
                this->stm->Calculate_IF_EXP();//THEN后面还可以跟GOTO符号。
            } catch (text_error error) {
                print("Line:"+QString::number(this->stm->Line())+"\t"+error.get_error_information());
                emit this->error();
                return;
            }
        }

        return;
    }
    if(this->St==END)
    {
      return;
    }
    text_error error("exist UNKNOWN statements.");
    print(error.get_error_information());
    emit this->error();
}
int tokenizer::Current_Line_index()
{
    return this->stm->Line();
}
