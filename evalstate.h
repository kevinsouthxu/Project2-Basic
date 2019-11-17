#ifndef EVALSTATE_H
#define EVALSTATE_H
#include <QString>
#include <QMap>
class EvaluationContext
{
public:

    void setValue(QString var,int value);
    int getValue(QString var);
    bool isDefined(QString var);
    void clear();

private:

    QMap<QString,int> symbolTable;
};
#endif // EVALSTATE_H
