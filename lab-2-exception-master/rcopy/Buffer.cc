#include "Buffer.h"

// TODO: add implementation of class Buffer
Buffer::Buffer(int size) {
      
      buffer =(char *)malloc(size);
      if(buffer==NULL)
       {
          throw Exception(1);
       }
       
}
Buffer::~Buffer(){
   
    free(buffer);

     
}
void Buffer::Reversing_Buffer(int length){
     if(length<0)
     {
         throw Exception(1);
     }
     else{
      int start=0;int end=length-1;
      char Data;
      while(start<end)
      {
          Data = buffer[start];
          buffer[start] = buffer[end];
          buffer[end]=Data;
          start++;
          end--;
      }
     }
   
}