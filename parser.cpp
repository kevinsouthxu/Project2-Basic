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
exp* Parser::returnexp(){
    return this->head;
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
    postread(head);
}
void Parser::postread(exp *tmp){
    if(tmp==nullptr) return;
    postread(tmp->Left);
    postread(tmp->Right);
    if(tmp->ex->type()==CONSTANT) {pre.append(tmp->ex->toString());helper.enqueue(tmp->ex);}
    if(tmp->ex->type()==IDENTIFIER) {pre.append(tmp->ex->toString());helper.enqueue(tmp->ex);}
    if(tmp->ex->type()==COMPOUND) {pre.append(tmp->ex->toString());helper.enqueue(tmp->ex);}
}
int Parser::Calculate(EvaluationContext *context,exp *hd){
    helper.clear();
    calculator.clear();
    this->postread(hd);//后缀放在helper队列里了
    while(!helper.empty())
    {
        if(helper.front()->type()==IDENTIFIER||helper.front()->type()==CONSTANT)
        {
            calculator.push(helper.front());
            helper.pop_front();
            continue;
        }
        if(helper.front()->type()==COMPOUND)//还未做好异常处理
        {
            Expression *Cal_exp1=calculator.pop();
            Expression *Cal_exp2=calculator.pop();
            QString temp_op=helper.front()->toString();
            if (temp_op=="+")
            {
                int calculate_result=Cal_exp2->eval(context)+Cal_exp1->eval(context);
                Expression *tmp=new ConstantExp(calculate_result);
                calculator.push(tmp);
                helper.pop_front();
                continue;
            }
            if (temp_op=="-")
            {
                int calculate_result=Cal_exp2->eval(context)-Cal_exp1->eval(context);
                Expression *tmp=new ConstantExp(calculate_result);
                calculator.push(tmp);
                helper.pop_front();
                continue;
            }
            if (temp_op=="*")
            {
                int calculate_result=Cal_exp2->eval(context)*Cal_exp1->eval(context);
                Expression *tmp=new ConstantExp(calculate_result);
                calculator.push(tmp);
                helper.pop_front();
                continue;
            }
            if (temp_op=="/")
            {
                int calculate_result=Cal_exp2->eval(context)/Cal_exp1->eval(context);
                Expression *tmp=new ConstantExp(calculate_result);
                calculator.push(tmp);
                helper.pop_front();
                continue;
            }
            if (temp_op=="=")
            {
                if(Cal_exp2->type()!=IDENTIFIER)
                    throw "Unvalid Expression.";
                context->setValue(Cal_exp2->toString(),Cal_exp1->eval(context));
                calculator.push(Cal_exp2);
                helper.pop_front();
                continue;
            }

        }
    }
    if(calculator.front()->type()==IDENTIFIER||calculator.front()->type()==CONSTANT)
    {
        if(calculator.empty())
            throw "UNVALID Expression.";
        Expression *tempexp=calculator.pop();
            return tempexp->eval(context);
    }

}
