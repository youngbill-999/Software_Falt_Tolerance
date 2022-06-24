// TODO: Implement all methods of class File
#include "File.h"

//

//

File::File(const char *name, const char *mode) {
     file = fopen(name,mode); 
     if(file==NULL)
     {
         throw Exception(2);
     }
}

int File::read(Buffer &buffer, int size) {
    int read = fread(buffer.buffer,1,size,file);
    if(read==-1)
    {
        throw Exception(1);
    }
    else
    {
       return read;
    }

}

void File::write(Buffer &buffer, int size) {
       if(size==0)
       {
           fwrite(buffer.buffer,1,size,file);
       }
       else{
       int i=0;
       i=fwrite(buffer.buffer,1,size,file);
       if(i<=0)
       {
           throw  Exception(2);
       }   
       } 
}

void File::seek(int pos) {
    if(pos<0)
    {
        throw Exception(2);
    }
    else
    {
        fseek(file,pos,SEEK_SET);
    }
}

void File::close() {
    if(file!=NULL)
    fclose(file);
    else
    {
        throw Exception(2);
    }
    
}

int File::aclength(){
    int i = fseek(file,0,SEEK_END);
    if(i<0){throw Exception(2);}
    else{
          length = ftell(file);
          fseek(file,0,SEEK_SET);
          return length;
    }
}
