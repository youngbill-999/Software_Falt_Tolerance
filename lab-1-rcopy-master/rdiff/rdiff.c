#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

int main (int argc, char** argv) {
    char *buffer;char *bufferrev; FILE *f1, *f2;

    buffer = malloc (BUFFER_SIZE);
    bufferrev = malloc (BUFFER_SIZE);
    f1 = fopen (argv[1], "r");
    f2 = fopen (argv[2], "r");

    fseek(f1, 0, SEEK_END);
    int length=ftell(f1);
    fseek(f1, 0, SEEK_SET);

    fseek(f2, 0, SEEK_END);
    int lengthrev=ftell(f2);
    fseek(f2, 0, SEEK_SET);
    
    if(length!=lengthrev){
	printf("fail!");			
	fclose (f1); fclose (f2);
	free (buffer);free (bufferrev);
	return 1;
    }
    
	int size=0;
    if(lengthrev>BUFFER_SIZE){
        size++;
        fseek(f2,lengthrev-size*BUFFER_SIZE,SEEK_SET); 
    }else{
        size++;
    }
    int read1 = fread (buffer, 1, BUFFER_SIZE, f1);
    int read2 = fread (bufferrev, 1, BUFFER_SIZE, f2);
    while (read2!=-1) {
        for(int i=0;i<read1;i++){
	    if(buffer[i]!=bufferrev[read1-1-i]){
		printf("fail!");			
		fclose (f1); fclose (f2);
		free (buffer);free (bufferrev);
		return 1;
	    }	
	}
         if((lengthrev-size*BUFFER_SIZE)>BUFFER_SIZE){
            size++;
            fseek(f2,lengthrev-size*BUFFER_SIZE,SEEK_SET);
            read1 = fread (buffer, 1, BUFFER_SIZE, f1);
	    read2 = fread (bufferrev, 1, BUFFER_SIZE, f2);
        }else if((lengthrev-BUFFER_SIZE*size)>0){
            fseek(f2,0,SEEK_SET);
            read1=fread(buffer, 1, lengthrev-BUFFER_SIZE*size,f1);
	    read2 = fread (bufferrev, 1, BUFFER_SIZE, f2);
            size++;
        }else
        {
            break;
        }
    }
    fclose (f1); fclose (f2);
    free (buffer);free (bufferrev);
    return 0;
}

