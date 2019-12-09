#include "statement.h"
#include "Parser.h"
#include "text_error.h"
#include <QChar>
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
    text_error error("CAN'T get statement.");
    throw error;
}
QString statement::returnINPUT(){
    text_error error("CAN'T get statement.");
    throw error;
}
void statement::parseIF_EXP(){
    text_error error("CAN'T get statement.");
    throw error;
}
QString statement::returnIF(){
    text_error error("CAN'T get statement.");
    throw error;
}
void statement::getContext(EvaluationContext *con){
    text_error error("CAN'T get statement.");
    throw error;
}
void statement::CalculateEXP(){
    text_error error("CAN'T get statement.");
    throw error;
}
void statement::Calculate_IF_EXP(){
    text_error error("CAN'T get statement.");
    throw error;
}
int statement::returnPRINT(){
    text_error error("CAN'T get statement.");
    throw error;
}
int statement::returnGOTO(){
    text_error error("CAN'T get statement.");
    throw error;
}
int statement::IF_Judge_Condition(){
    text_error error("CAN'T get statement.");
    throw error;
}
bool statement::returnIF_bool(){
    text_error error("CAN'T get statement.");
    throw error;
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
    catch(text_error e)
    {
        throw(e);
    }
    this->head_LET=par->returnexp();
    par->postread();
    preOrder=par->pre;
    this->head_LET=par->returnexp();
    return par->pre;
}
void LETstatement::getContext(EvaluationContext *con){
    this->context=con;
}
void LETstatement::CalculateEXP(){
    Parser *par=new Parser;
    int t=par->Calculate(this->context,this->head_LET);
}


PRINTstatement::PRINTstatement(int line){
    this->linenumber=line;
    head=new token;
    rear=new token;
    currenttok=new token;
    head->next=rear;
    rear->tokens="\0";
    PRINT_Number=0;
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
    catch(text_error error)
    {
       throw(error);
    }
    this->head_PRINT=par->returnexp();
    par->preread();
    preOrder=par->pre;
    this->head_PRINT=par->returnexp();
    return par->pre;
}
int PRINTstatement::Line(){
    return linenumber;
}
void PRINTstatement::getContext(EvaluationContext *con){
    this->context=con;
}
void PRINTstatement::CalculateEXP(){
    Parser *par=new Parser;
    try {
        PRINT_Number=par->Calculate(this->context,this->head_PRINT);
    } catch (text_error error) {
        throw(error);
    }
}
int PRINTstatement::returnPRINT(){
    return this->PRINT_Number;
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
    if(hd->tokens=="")
    {
        text_error error("The INPUT should be a valid variable.");
        throw error;
    }
    while(hd->tokens=="\0")
        hd=hd->next;
    if(hd->next->tokens!="\0"||(hd->tokens.at(0)>='0'&&hd->tokens.at(0)<='9'))          //按理来说hd的next就是rear,rear存的是\0
       {
        text_error error("The INPUT should be a valid variable.");
        throw error;
    }
    for(int i=0;i<hd->tokens.length();i++)
    {
        if(!(hd->tokens.at(i)>='0'&&hd->tokens.at(i)<='9')&&
           !((hd->tokens.at(i)>='a'&&hd->tokens.at(i)<='z'))&&
           !(hd->tokens.at(i)>='A'&&hd->tokens.at(i)<='Z')&&
           (hd->tokens.at(i)!='_'))
        {
            text_error error("The INPUT should be a valid variable.");
            throw error;
        }

    }
    INPU=hd->tokens;
    return;
}
void INPUTstatement::getContext(EvaluationContext *con){
    this->context=con;
}


GOTOstatement::GOTOstatement(int line){
    this->linenumber=line;
    gotoline=0;
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
    if(hd->tokens=="")
    {
        text_error error("The GOTO should be a integer.");
         throw error;
    }
    if(hd->next->tokens!="\0")          //按理来说hd的next就是rear,rear存的是\0
    {
       text_error error("The GOTO should be a integer.");
        throw error;
    }
    gotoline=hd->tokens.toInt(&ok);
    if(!ok)
    {
       text_error error("The GOTO should be a integer.");
        throw error;
    }
    return;
}
int GOTOstatement::returnGOTO(){
    return gotoline;
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

    t1_1=0;
    t1_2=0;
    t=0;
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
    if(hd->tokens=="")
    {
        text_error error("UNVALID IF grammer.");
         throw error;
    }
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
    if(head2->next->tokens=="THEN")
    {
        bool ok;
        int test=head2->next->next->tokens.toInt(&ok);
        if(ok)
        {
            text_error error("The GOTO should be a integer.");
             throw error;
        }
    }
    try{
    par->Build_Tree(head2->next);
    }
    catch(text_error error)
    {
        throw error;
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
    catch(text_error error)
    {
        throw error;
    }
    par->postread();
    preOrder1_1=par->pre;
    this->head2_IF=par->returnexp();

    par=new Parser;
    try{
    par->Build_Tree(head1_2->next);
    }
    catch(text_error error)
    {
         throw error;
    }
    par->postread();
    preOrder1_2=par->pre;
    this->head3_IF=par->returnexp();
}
QString IFstatement::returnIF(){
    return head2->next->tokens;
}
int IFstatement::returnGOTO(){
    bool ok;
    int a=head2->next->next->tokens.toInt(&ok);
    if(!ok)
        throw("The GOTO should be a integer.");
    return a;

}
void IFstatement::getContext(EvaluationContext *con){
    this->con=con;
}
void IFstatement::CalculateEXP(){
    Parser *par=new Parser;
    try {
        t1_1=par->Calculate(this->con,this->head2_IF);
    } catch (text_error error) {
        throw error;
    }
    par=new Parser;
    try {
        t1_2=par->Calculate(this->con,this->head3_IF);
    } catch (text_error error) {
        throw error;
    }
}
void IFstatement::Calculate_IF_EXP(){
    Parser *par=new Parser;
    t=par->Calculate(this->con,this->head1_IF);
}
int IFstatement::IF_Judge_Condition(){
    currenttok1=head1->next;
    int count1=0;
    int count2=0;
    while(currenttok1!=rear1)
    {
        if(currenttok1->tokens==">"||currenttok1->tokens=="<")
            count1++;
        if(currenttok1->tokens=="=")
            count2++;
        currenttok1=currenttok1->next;
    }
    if((count1>1||count2>1)||(count1==0&&count2==0))
        return 0;
    if(count1==1&&count2==1)
    {
        currenttok1=head1->next;
        while(currenttok1!=rear1&&currenttok1->tokens!=">"&&currenttok1->tokens!="<")
        {
            currenttok1=currenttok1->next;
        }
        currenttok1=currenttok1->next;
        if(currenttok1->tokens!="=")
        {
            text_error error("The IF condition's two exps shouldn't have equal operator.");
            throw error;
        }
    }
    currenttok1=head1->next;
    while(currenttok1!=rear1&&currenttok1->tokens!=">"&&currenttok1->tokens!="<"&&currenttok1->tokens!="=")
    {
        currenttok1=currenttok1->next;
    }
    if(currenttok1->tokens=="=")
        return 1;
    if(currenttok1->tokens==">"||currenttok1->tokens=="<")
    {
        if(currenttok1->next->tokens=="=")
        {
            if(currenttok1->tokens==">")
                return 4;
            if(currenttok1->tokens=="<")
                return 5;
        }
        if(currenttok1->tokens==">")
            return 2;
        if(currenttok1->tokens=="<")
            return 3;
    }
}
bool IFstatement::returnIF_bool(){
    int IF_type=this->IF_Judge_Condition();
    if(IF_type==0)
    {
        text_error error("UNVALID IF grammer.");
        throw error;
        return false;
    }
    if(IF_type==1)
    {
        if(this->t1_1==this->t1_2)
            return true;
        return false;
    }
    if(IF_type==2)
    {
        if(this->t1_1>this->t1_2)
            return true;
        return false;
    }
    if(IF_type==3)
    {
        if(this->t1_1<this->t1_2)
            return true;
        return false;
    }
    if(IF_type==4)
    {
        if(this->t1_1>=this->t1_2)
            return true;
        return false;
    }
    if(IF_type==5)
    {
        if(this->t1_1<=this->t1_2)
            return true;
        return false;
    }
    return false;
}


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
     {
        text_error error("The END should only have END.");
        throw error;
    }
    return;
}
