#ifndef __EXECPTION_H__
#define __EXECPTION_H__
#include <cerrno>
#include "Buffer.h"
#include <iostream>
using namespace std;

class Exception {
    // TODO: add private and public Members
   public:
    int error_code; 
    char *message;         
    const char *getMessage();//const是不变量的意思
    int getErrno();
    Exception(int e);
};

#endif