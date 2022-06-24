#include "File.h"
#include "Buffer.h"
#include "Exception.h"
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
using namespace std;
#define BUFFER_SIZE 1024
//

int main(const int argc, const char **argv) {
    try{
    Buffer b(BUFFER_SIZE);
    File f1(argv[1],"r");
    File f2(argv[2],"w");

    int Size=0;
    int length = f1.aclength();

    if(length>BUFFER_SIZE){
        Size++;
        f1.seek(length-Size*BUFFER_SIZE);
    }else{
        Size++;
    }
    
    int read=f1.read(b,BUFFER_SIZE);//此时的指针在倒数第一个Size的头部

    while(read!=1)
    {
        b.Reversing_Buffer(read);
        f2.write(b,read);

        if((length-Size*BUFFER_SIZE)>BUFFER_SIZE)
        {
            Size++;
            f1.seek(length-Size*BUFFER_SIZE);
            read=f1.read(b,BUFFER_SIZE);
        }else if((length-Size*BUFFER_SIZE)>0){
            f1.seek(0);
            read=f1.read(b,length-Size*BUFFER_SIZE);
            Size++;
        }else 
           break;

    }

    //b.~Buffer();
    f1.close();
    f2.close();
    }
    catch(int exe)
    {
        Exception Exc(exe);
        cout<<"the kind of failure is:"<<Exc.getErrno()<<","<<Exc.getMessage();
    }
    return 0;
}
