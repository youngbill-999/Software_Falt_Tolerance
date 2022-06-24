#include <iostream>
#include <fstream>
#define BUFFER_SIZE 1024
using namespace std;

int main(const int argc, const char **argv)
{
    char buffer[BUFFER_SIZE];
    char Data;
    int size=0;

    ifstream inputFile;
    inputFile.open(argv[1]);
    ofstream outFile;
    outFile.open(argv[2]);

    int start = inputFile.tellg();
    inputFile.seekg (0, ios::end);
    int end = inputFile.tellg();
    inputFile.seekg (0, ios::beg);
    int length =end-start;

    
    if(length>BUFFER_SIZE){
        size++;
        inputFile.seekg(length-size*BUFFER_SIZE,ios::beg);
    }else{
        size++;
    }
    int read =inputFile.read(buffer,BUFFER_SIZE).gcount();

    while(read!=-1) {
        int end=read-1;
        int beg=0;
        while(beg<end){
            Data=buffer[beg];
            buffer[beg]=buffer[end];
            buffer[end]=Data;
            beg++;
            end--;
        }
        outFile.write(buffer,read);
        if((length-size*BUFFER_SIZE)>BUFFER_SIZE){
            size++;
            inputFile.seekg(length-size*BUFFER_SIZE,ios::beg);
            read=inputFile.read(buffer,BUFFER_SIZE).gcount();
        }else if ((length-size*BUFFER_SIZE)>0){
            inputFile.seekg(0,ios::beg);
            read=inputFile.read(buffer,length-size*BUFFER_SIZE).gcount();
            size++;
        }else{
            break;
        }
    }
    inputFile.close();
    outFile.close();
    return 0;
}

