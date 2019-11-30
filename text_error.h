#ifndef TEXT_ERROR_H
#define TEXT_ERROR_H
#include <QString>
//用于处理错误的类
class text_error
{
public:
    text_error(QString);
    QString get_error_information();
private:
    QString error_information;
};

#endif // TEXT_ERROR_H
