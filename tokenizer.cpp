#include "tokenizer.h"
#include "parser.h"
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
                tmp=new token;
                tmp->next=currenttoken->next;
                currenttoken->next=tmp;
                currenttoken=currenttoken->next;
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
                    tmp=new token;
                    tmp->next=currenttoken->next;
                    currenttoken->next=tmp;
                    currenttoken=currenttoken->next;

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
        QString t(this->stm->parseEXP());
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
        catch (const char *error)
         {
             print("Line: "+QString::number(this->stm->Line())+"\t"+error);
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
         catch (const char *error)
          {
             print("Line: "+QString::number(this->stm->Line())+"\t"+error);
          }
         return;
     }
     if(tmp->tokens=="IF")
     {
        this->St=IF;
        this->stm=new IFstatement(head->tokens.toInt());
        this->stm->gettoken(tmp->next);
        this->stm->parseIF_EXP();
        QString t(this->stm->returnIF());
         if(stm->have_THEN())
          QString t1(this->stm->parseEXP());

         return;
     }
     if(tmp->tokens=="END")
     {
        this->St=END;
        this->stm=new ENDstatement(head->tokens.toInt());
         try{
         this->stm->gettoken(tmp->next);
          }
         catch (QString error)
          {
              print("Line: "+QString::number(this->stm->Line())+"\t"+error);
          }
         return;
     }
}

void tokenizer::RUN()
{
    if(this->St==LET)
    {

    }
    if(this->St==PRINT)
    {

    }
    if(this->St==INPUT)
    {
        eva->setValue(this->stm->returnINPUT(),0);///////
    }
}
