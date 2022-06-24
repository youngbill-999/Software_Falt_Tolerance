#include "Exception.h"

// TODO: add implementation of class Exception
int Exception::getErrno() {
   return error_code;
}

const char *Exception::getMessage() {
   return message;
}
Exception::Exception(int e)
{
    error_code = e;
    if(e==1)
    {
        message="fail!";
    }
    if(e==2)
    {
        message = "fault!";
    }
}
