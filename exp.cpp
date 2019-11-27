#include "exp.h"

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
    throw "error: The Value is nonexistent.";
}
QString Expression::getIdentiferName(){
    throw "error: The Identifier is nonexistent.";
}
QString Expression::getOperator(){
    throw "error: The Operator is nonexistent.";
}
Expression *Expression::getLHS(){
    throw "error: The LHS is nonexistent.";
}
Expression *Expression::getRHS(){
    throw "error: The RHS is nonexistent.";
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
    if(!context->isDefined(name)) throw(name + " is undefined.");
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
        if(right == 0) throw("Division by 0");
        return left / right;
    }
    throw("Illegal operator in expression");
    return 0;
}


