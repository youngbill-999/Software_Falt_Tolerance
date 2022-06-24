#ifndef __FILE_H__
#define __FILE_H__

#include "Buffer.h"
//#include <iostream>
#include <fstream>
#include <unistd.h>
#include "unistd.h"
//using namespace std;

class File {
    // TODO: add public and private members
  public:
    // Note: opening the same file twice for writing ("w")
    // at the same time is forbidden
    FILE *file;
    int length;

    File(const char *name, const char *mode);
    int read(Buffer &buffer, int size);
    void write(Buffer &buffer, int size);
    void seek(int pos);
    void close();

    int aclength();
};

#endif
