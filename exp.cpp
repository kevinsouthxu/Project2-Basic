#include "exp.h"
#include "text_error.h"

Expression::Expression(){
}
Expression::~Expression(){
}
/*
int Expression::eval(EvaluationContext & context){
    return 0;
}
ExpressionType Expression::type(){

}
QString Expression::toString(){
    return "hello";
}
*/
int Expression::getConstantValue(){
    text_error error("The Value is nonexistent.");
    throw error;
}
QString Expression::getIdentiferName(){
    text_error error("The Identifier is nonexistent.");
    throw error;
}
QString Expression::getOperator(){
    text_error error("The Operator is nonexistent.");
    throw error;
}
Expression *Expression::getLHS(){
    text_error error("The LHS is nonexistent.");
    throw error;;
}
Expression *Expression::getRHS(){
    text_error error("The RHS is nonexistent.");
    throw error;
}
//

ConstantExp::ConstantExp(int val){
    this->value=val;
}

int ConstantExp::eval(EvaluationContext *context){
    return this->value;
}

QString ConstantExp::toString(){
    QString str=QString::number(value);
    return str;
}

int ConstantExp::getConstantValue(){
    return this->value;
}

ExpressionType ConstantExp::type(){
    return CONSTANT;
}
//

IdentifierExp::IdentifierExp(QString name){
    this->name=name;
}

ExpressionType IdentifierExp::type(){
    return IDENTIFIER;
}

QString IdentifierExp::toString(){
    return this->name;
}

QString IdentifierExp::getIdentifierName(){
    return this->name;
}

int IdentifierExp::eval(EvaluationContext *context){
    if(!context->isDefined(name))
    {
        text_error error(name+" is undefined.");
        throw error;
     }
    return context->getValue(name);
}
//

CompoundExp::CompoundExp(QString op,Expression *lhs,Expression *rhs){
    this->op=op;
    this->lhs=lhs;
    this->rhs=rhs;
}

CompoundExp::~CompoundExp(){
    delete lhs;
    delete rhs;
}

QString CompoundExp::toString(){
    return op;
}

ExpressionType CompoundExp::type(){
    return COMPOUND;
}

QString CompoundExp::getOperator(){
    return op;
}

Expression  *CompoundExp::getLHS(){
    return this->lhs;
}

Expression  *CompoundExp::getRHS(){
    return this->rhs;
}

int CompoundExp::eval(EvaluationContext *context){
    int right = rhs->eval(context);
    if(op == "=") {
        context->setValue(lhs->getIdentiferName(),right);
        return right;
    }
    int left = lhs->eval(context);
    if(op == "+") return left + right;
    if(op == "-") return left - right;
    if(op == "*") return left * right;
    if(op == "/") {
        if(right == 0) {
            text_error error("Division by 0");
            throw error;
        }
        return left / right;
    }
    text_error error("Illegal operator in expression");
    throw error;
    return 0;
}


