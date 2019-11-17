#include "parser.h"
Parser::Parser(){
    operands.clear();
    operators.clear();
    calculator.clear();
    helper.clear();
    head=new exp();
    head->Left=nullptr;
    head->Right=nullptr;
    pre.clear();
}
Parser::~Parser(){
    operands.clear();
    operators.clear();
    calculator.clear();
    pre.clear();
}
void Parser::Build_Tree(token *hd){
    exp *temp;
    int cons; //表达式的数字
    while(hd->tokens!="\0")
    {
        if(hd->tokens!="+"
         &&hd->tokens!="-"
         &&hd->tokens!="*"
         &&hd->tokens!="/"
         &&hd->tokens!="="
         &&hd->tokens!=">"
         &&hd->tokens!="<"
         &&hd->tokens!="("
         &&hd->tokens!=")")
        {
            bool ok;
            cons=hd->tokens.toInt(&ok);
            if(ok)
            {
                temp=new exp;
                temp->ex=new ConstantExp(cons);
                temp->Left=nullptr;
                temp->Right=nullptr;
                operands.push(temp);
            }
            else
            {
                if(hd->tokens.at(0)>='0'&&hd->tokens.at(0)<='9')
                        throw "Invalid naming.";
                temp=new exp;
                temp->ex=new IdentifierExp(hd->tokens);
                temp->Left=nullptr;
                temp->Right=nullptr;
                operands.push(temp);
            }
        }
        else
        {
            if(hd->tokens=="=")
            {
                while(!operators.empty()&&operators.top()!="(")
                {
                    temp=new exp;
                    temp->Right=operands.pop();
                    temp->Left=operands.pop();
                    temp->ex=new CompoundExp(operators.pop(),temp->Left->ex,temp->Right->ex);
                    operands.push(temp);
                }
                operators.push(hd->tokens);
            }
            if(hd->tokens=="+"||hd->tokens=="-")
            {
                while(!operators.empty()&&(operators.top()=="+"||operators.top()=="-"||operators.top()=="*"||operators.top()=="/"))
                {
                    temp=new exp;
                    temp->Right=operands.pop();
                    temp->Left=operands.pop();
                    temp->ex=new CompoundExp(operators.pop(),temp->Left->ex,temp->Right->ex);
                    operands.push(temp);
                }
                operators.push(hd->tokens);
            }
            if(hd->tokens=="*"||hd->tokens=="/")
            {
                while(!operators.empty()&&(operators.top()=="*"||operators.top()=="/"))
                {
                    temp=new exp;
                    temp->Right=operands.pop();
                    temp->Left=operands.pop();
                    temp->ex=new CompoundExp(operators.pop(),temp->Left->ex,temp->Right->ex);
                    operands.push(temp);
                }
                operators.push(hd->tokens);
            }
            if(hd->tokens=="(")
                operators.push(hd->tokens);
            if(hd->tokens==")")
            {
                while(!operators.empty()&&(operators.top()!="("))
                {
                    temp=new exp;
                    temp->Right=operands.pop();
                    temp->Left=operands.pop();
                    temp->ex=new CompoundExp(operators.pop(),temp->Left->ex,temp->Right->ex);
                    operands.push(temp);
                }
                operators.pop();
            }
        }
        hd=hd->next;
    }
    while(!operators.empty())
    {
        temp=new exp;
        temp->Right=operands.pop();
        temp->Left=operands.pop();
        temp->ex=new CompoundExp(operators.pop(),temp->Left->ex,temp->Right->ex);
        operands.push(temp);
    }
    head=operands.pop();
}
void Parser::preread(){
    helper.clear();
    pre.clear();
    preread(head);
}
void Parser::preread(exp *tmp){
    if(tmp==nullptr) return;
    if(tmp->ex->type()==CONSTANT) {pre.append(tmp->ex->toString());helper.enqueue(tmp->ex);}
    if(tmp->ex->type()==IDENTIFIER) {pre.append(tmp->ex->toString());helper.enqueue(tmp->ex);}
    if(tmp->ex->type()==COMPOUND) {pre.append(tmp->ex->toString());helper.enqueue(tmp->ex);}
    preread(tmp->Left);
    preread(tmp->Right);
}
void Parser::postread(){
    helper.clear();
    pre.clear();
    preread(head);
}
void Parser::postread(exp *tmp){
    if(tmp==nullptr) return;
    preread(tmp->Left);
    preread(tmp->Right);
    if(tmp->ex->type()==CONSTANT) {pre.append(tmp->ex->toString());helper.enqueue(tmp->ex);}
    if(tmp->ex->type()==IDENTIFIER) {pre.append(tmp->ex->toString());helper.enqueue(tmp->ex);}
    if(tmp->ex->type()==COMPOUND) {pre.append(tmp->ex->toString());helper.enqueue(tmp->ex);}
}
void Parser::Calculate(EvaluationContext *context,exp *hd){
    helper.clear();
    calculator.clear();
    this->postread(hd);
    while(!helper.empty())
    {
        if(helper.front()->type()==COMPOUND)
        {
            calculator.push(helper.front());
            helper.pop_front();
        }
        if(helper.front->type()==IDENTIFIER||helper.front->type()==CONSTANT)
        {
            if(calculator.top()->type()==COMPOUND)
            {
                calculator.push(helper.front());
                helper.pop_front();
            }
            else
            {

            }
        }

    }
}
