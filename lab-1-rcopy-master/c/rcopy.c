#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

int main (int argc, char** argv) {
    char *buffer; FILE *f1, *f2;

    buffer = malloc (BUFFER_SIZE);
    f1 = fopen (argv[1], "r");
    f2 = fopen (argv[2], "w");

    int Size=0;
     
    fseek(f1, 0, SEEK_END);
    int length=ftell(f1);
    fseek(f1, 0, SEEK_SET);
    
    if(length>BUFFER_SIZE){
        Size++;
        fseek(f1,length-Size*BUFFER_SIZE,SEEK_SET); 
    }else{
        Size++;
    }
    
    int read = fread (buffer, 1, BUFFER_SIZE, f1);
    
    while (read!=-1) {
	    int start=0,end=read-1;
        char Data;
        while(start<end) {
             Data=buffer[start];
             buffer[start]=buffer[end];
             buffer[end]=Data;
             start++;
             end--;
        }
        fwrite (buffer, 1, read, f2);

        if((length-Size*BUFFER_SIZE)>BUFFER_SIZE){
            Size++;
            fseek(f1,length-Size*BUFFER_SIZE,SEEK_SET);
            read = fread (buffer, 1, BUFFER_SIZE, f1);
        }else if((length-BUFFER_SIZE*Size)>0){
            fseek(f1,0,SEEK_SET);
            read=fread(buffer, 1, length-BUFFER_SIZE*Size,f1);
            Size++;
        }else
            break;
    }
    fclose (f1); fclose (f2);
    free (buffer);
    return 0;
}

