#include "evalstate.h"
void EvaluationContext::setValue(QString var,int value){
    symbolTable.insert(var,value);
}

int EvaluationContext::getValue(QString var){
    QMap<QString,int>::iterator mi;
    mi=symbolTable.find(var);
    return mi.value();
}

bool EvaluationContext::isDefined(QString var){
    return symbolTable.contains(var);
}

void EvaluationContext::clear(){
    symbolTable.clear();
}
