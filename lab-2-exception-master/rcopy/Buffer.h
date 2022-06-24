#ifndef __BUFFER_H__
#define __BUFFER_H__
#include <iostream>
#include "Exception.h"


class Buffer {
    // TODO: add public and private members
  public:
    char *buffer;
    
    Buffer(int size);
    ~Buffer();
    void Reversing_Buffer(int length);
    friend class File;
};

#endif