#include "text_error.h"
text_error::text_error(QString str)
{
    this->error_information=str;
}
QString text_error::get_error_information()
{
    return this->error_information;
}
