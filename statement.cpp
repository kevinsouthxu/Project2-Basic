#include "statement.h"
#include "Parser.h"
statement::statement(){

}
statement::~statement(){

}
void statement::gettoken(const token *hd){
    throw "error: CAN'T get statement.";
}
bool statement::have_THEN(){
    throw "error: CANT get statement.";
}
QString statement::parseEXP(){
    throw "error: CANT get statement.";
}
QString statement::returnINPUT(){
    throw "error: CANT get statement.";
}
void statement::parseIF_EXP(){
    throw "error: CANT get statement.";
}
QString statement::returnIF(){
    throw "error: CANT get statement.";
}


REMstatement::REMstatement(int line){
    this->linenumber=line;
    head=new token;
    rear=new token;
    currenttok=new token;
    head->next=rear;
    rear->tokens="\0";
}
REMstatement::~REMstatement(){

}
void REMstatement::gettoken(const token *hd){
    token *temp;
    currenttok->next=rear;
    head->next=currenttok;
    while(hd->tokens!="\0")
    {
        currenttok->tokens=hd->tokens;
        temp=new token;
        temp->next=currenttok->next;
        currenttok->next=temp;
        currenttok=currenttok->next;
        hd=hd->next;
    }
}
statementtype REMstatement::type(){
    return REM;
}
int REMstatement::Line(){
    return linenumber;
}


LETstatement::LETstatement(int line){
    this->linenumber=line;
    head=new token;
    rear=new token;
    currenttok=new token;
    head->next=rear;
    rear->tokens="\0";
}
LETstatement::~LETstatement(){

}
void LETstatement::gettoken(const token *hd){
    token *temp;
    currenttok->next=rear;
    head->next=currenttok;
    while(hd->tokens!="\0")
    {
        currenttok->tokens=hd->tokens;
        temp=new token;
        temp->next=currenttok->next;
        currenttok->next=temp;
        currenttok=currenttok->next;
        hd=hd->next;
    }
}
statementtype LETstatement::type(){
    return LET;
}
int LETstatement::Line(){
    return linenumber;
}
QString LETstatement::parseEXP(){
    Parser *par=new Parser;
    try{
    par->Build_Tree(head->next);
    }
    catch(QString error)
    {
        return error;
    }
    par->preread();
    preOrder=par->pre;
    this->head_LET=par->returnexp();
    return par->pre;
}


PRINTstatement::PRINTstatement(int line){
    this->linenumber=line;
    head=new token;
    rear=new token;
    currenttok=new token;
    head->next=rear;
    rear->tokens="\0";
}
PRINTstatement::~PRINTstatement(){

}
void PRINTstatement::gettoken(const token *hd){
    token *temp;
    currenttok->next=rear;
    head->next=currenttok;
    while(hd->tokens!="\0")
    {
        currenttok->tokens=hd->tokens;
        temp=new token;
        temp->next=currenttok->next;
        currenttok->next=temp;
        currenttok=currenttok->next;
        hd=hd->next;
    }
}
statementtype PRINTstatement::type(){
    return PRINT;
}
QString PRINTstatement::parseEXP(){
    Parser *par=new Parser;
    try{
    par->Build_Tree(head->next);
    }
    catch(const char *error)
    {
       QString temp;
       token *tmp=new token;
       tmp=head->next;
       while(tmp->tokens!="\0")
       {
           temp.append(tmp->tokens);
           tmp=tmp->next;
       }
       return temp;
    }
    par->preread();
    preOrder=par->pre;
    this->head_PRINT=par->returnexp();
    return par->pre;
}
int PRINTstatement::Line(){
    return linenumber;
}


INPUTstatement::INPUTstatement(int line){
    this->linenumber=line;
}
INPUTstatement::~INPUTstatement(){

}
statementtype INPUTstatement::type(){
    return INPUT;
}
int INPUTstatement::Line(){
    return linenumber;
}
void INPUTstatement::gettoken(const token *hd){
    while(hd->tokens=="\0")
        hd=hd->next;
    if(hd->next->tokens!="\0"||(hd->tokens.at(0)>='0'&&hd->tokens.at(0)<='9'))          //按理来说hd的next就是rear,rear存的是\0
        throw "The INPUT should be the value of a variable.";
    INPU=hd->tokens;
    return;
}


GOTOstatement::GOTOstatement(int line){
    this->linenumber=line;
}
GOTOstatement::~GOTOstatement(){

}
statementtype GOTOstatement::type(){
    return GOTO;
}
int GOTOstatement::Line(){
    return linenumber;
}
void GOTOstatement::gettoken(const token *hd){
    bool ok;
    if(hd->next->tokens!="\0")          //按理来说hd的next就是rear,rear存的是\0
        throw "The GOTO should be a integer.";
    gotoline=hd->tokens.toInt(&ok);
    if(!ok)
        throw "The GOTO should be a integer.";
    return;
}


IFstatement::IFstatement(int line){
    this->linenumber=line;

    head1=new token;
    rear1=new token;
    currenttok1=new token;
    head1->next=rear1;
    rear1->tokens="\0";

    head1_1=new token;
    rear1_1=new token;
    currenttok1_1=new token;
    head1_1->next=rear1_1;
    rear1_1->tokens="\0";

    head1_2=new token;
    rear1_2=new token;
    currenttok1_2=new token;
    head1_2->next=rear1_2;
    rear1_2->tokens="\0";

    head2=new token;
    rear2=new token;
    currenttok2=new token;
    head2->next=rear2;
    rear2->tokens="\0";
}
IFstatement::~IFstatement(){

}
statementtype IFstatement::type(){
    return IF;
}
int IFstatement::Line(){
    return linenumber;
}
void IFstatement::gettoken(const token *hd){
    token *temp;
    currenttok1->next=rear1;
    head1->next=currenttok1;
    while(hd->tokens!="THEN"&&hd->tokens!="\0")
    {
        currenttok1->tokens=hd->tokens;
        temp=new token;
        temp->next=currenttok1->next;
        currenttok1->next=temp;
        currenttok1=currenttok1->next;
        hd=hd->next;
    }
    if(hd->tokens=="THEN")
    {
        hd=hd->next;
        currenttok2->next=rear2;
        head2->next=currenttok2;
        while(hd->tokens!="\0")
        {
            currenttok2->tokens=hd->tokens;
            temp=new token;
            temp->next=currenttok2->next;
            currenttok2->next=temp;
            currenttok2=currenttok2->next;
            hd=hd->next;
        }
    }
}
bool IFstatement::have_THEN(){
    if(head2->next==rear2)
        return false;
    return true;
}
QString IFstatement::parseEXP(){
    Parser *par=new Parser;
    if(head2->next=="THEN")
    {
        bool ok;
        int test=head2->next->next->tokens.toInt(&ok);
        if(ok)
            return "GOTO"+head2->next->next;
        else
            throw "The GOTO should be a integer.";
    }
    try{
    par->Build_Tree(head2->next);
    }
    catch(const char *error)
    {
        return error;
    }
    par->preread();
    preOrder=par->pre;
    this->head1_IF=par->returnexp();
    return par->pre;
}
void IFstatement::parseIF_EXP(){
    Parser *par=new Parser;
    currenttok1=head1->next;
    token *temp;
    currenttok1_1->next=rear1_1;
    head1_1->next=currenttok1_1;
    while(currenttok1->tokens!="="&&currenttok1->tokens!="<"&&currenttok1->tokens!=">")
    {
        currenttok1_1->tokens=currenttok1->tokens;
        temp=new token;
        temp->next=currenttok1_1->next;
        currenttok1_1->next=temp;
        currenttok1_1=currenttok1_1->next;
        currenttok1=currenttok1->next;
    }
    currenttok1=currenttok1->next;
    if(currenttok1->tokens=="=") currenttok1=currenttok1->next;
    currenttok1_2->next=rear1_2;
    head1_2->next=currenttok1_2;
    while(currenttok1->tokens!="\0")
    {
        currenttok1_2->tokens=currenttok1->tokens;
        temp=new token;
        temp->next=currenttok1_2->next;
        currenttok1_2->next=temp;
        currenttok1_2=currenttok1_2->next;
        currenttok1=currenttok1->next;
    }
    try{
    par->Build_Tree(head1_1->next);
    }
    catch(const char *error)
    {
        preOrder1_1=*error;
    }
    par->preread();
    preOrder1_1=par->pre;
    this->head2_IF=par->returnexp();

    par=new Parser;
    try{
    par->Build_Tree(head1_2->next);
    }
    catch(const char *error)
    {
         preOrder1_2=*error;
    }
    par->preread();
    preOrder1_2=par->pre;
    this->head3_IF=par->returnexp();
}
QString IFstatement::returnIF(){
    return preOrder1_1+"\t"+preOrder1_2;
}
//

ENDstatement::ENDstatement(int line){
    this->linenumber=line;
}
ENDstatement::~ENDstatement(){

}
statementtype ENDstatement::type(){
    return END;
}
int ENDstatement::Line(){
    return linenumber;
}
void ENDstatement::gettoken(const token *hd){
    if(hd->tokens!="\0")
        throw "The END should only have END";
    return;
}
