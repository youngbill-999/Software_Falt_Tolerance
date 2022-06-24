#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

#include <string>
#include <iostream>
#include <stdlib.h>

#include "tools.h"
#include "tests.h"
#include "stats.h"

FILE *generateFILE(int test_id) {
    // generate test FILE* test value
    // see tests.h
    // use the functions specified by tools.h to create appropriate test values
    // you can use a copy of test.txt as a file to test on
    FILE *file;
    switch(test_id){
        case 0:
            file=NULL;
             break;
        case 1:
             filecopy("./test.txt", "./testcopy.txt");
            file = fopen ("./testcopy.txt","r");
             break;
        case 2:
             filecopy("./test.txt", "./testcopy.txt");
            file = fopen("./testcopy.txt","w");
             break;
        case 3:
            filecopy("./test.txt", "./testcopy.txt");
            file = fopen("./testcopy.txt","r+");
             break;
        case 4:
            filecopy("./test.txt", "./testcopy.txt");
            file = fopen("./testcopy.txt","r+");
            fclose(file);
             break;
        case 5:
            filecopy("./test.txt", "./testcopy.txt");
            file = (FILE *)malloc_prot(17,"memory read only",PROT_READ);//PROT_NONE, PROT_READ, and PROT_WRITE 
             break;
        case 6:
            filecopy("./test.txt", "./testcopy.txt");
            file = (FILE *)malloc_prot(18,"memory write only",PROT_WRITE);//PROT_NONE, PROT_READ, and PROT_WRITE 
             break;
        case 7:
             filecopy("./test.txt", "./testcopy.txt");
            file = (FILE *)malloc_prot(15,"read and write",PROT_WRITE|PROT_READ);//PROT_NONE, PROT_READ, and PROT_WRITE 
            break;
        case 8:
         filecopy("./test.txt", "./testcopy.txt");
            file = (FILE *)malloc_prot(0,NULLpage(),PROT_READ);//PROT_NONE, PROT_READ, and PROT_WRITE 
             break;
        case 9:
         filecopy("./test.txt", "./testcopy.txt");
             file =(FILE *)malloc_prot(0, NULLpage(),PROT_WRITE) ;
              break;
        case 10:
         filecopy("./test.txt", "./testcopy.txt");
             file =(FILE *)malloc_prot(0, NULLpage(),PROT_WRITE|PROT_READ) ;
              break;
        case 11:
             filecopy("./test.txt", "./testcopy.txt");
             file = (FILE *)malloc_prot(0, NULLpage(),PROT_NONE);
             break;
             
    }
        return file;
    


}

const char *generateCSTR(int test_id) {
    // generate a `const char*` test value
    // see tests.h
    // use the functions specified by tools.h to create appropriate test values
    char* CSTR;
    char string[11] = {'1','2','3', '4','5', '6', '7', '8', '9', '0', '\0'};
        switch(test_id)
    {
        case 0:
         CSTR =NULL;
         break;
        case 1:
         CSTR = (char *)malloc_prot(sizeof(string),string,PROT_READ);
         break;
        case 2:
         CSTR = (char *)malloc_prot(sizeof(string),string,PROT_WRITE);
         break;
        case 3:
         CSTR = (char *)malloc_prot(sizeof(string),string,PROT_READ|PROT_WRITE);
         break;
        case 4:
         CSTR = (char *)malloc_prot(sizeof(string)-1,string,PROT_READ);
         break;
        case 5:
         CSTR = (char *)malloc_prot(sizeof(string)-1,string,PROT_WRITE);//termination string
         break;
        case 6:
         CSTR = (char *)malloc_prot(sizeof(string)-1,string,PROT_READ|PROT_WRITE);
         break;
        case 7:
          CSTR = (char *)malloc_prot(sizeof(string),string,PROT_NONE);
          break;
    }
    return CSTR;
   
}

// waiting time before querying the child's exit status
// You might want to try using a smaller value in order to get the CI results faster,
// but there is a chance that your tests will start failing because of the timeout
const double wait_time = 1.0;
void test_fputs(const TestCase &str_testCase, const TestCase &file_testCase) {
    // execute a single test
    // use the functions in stats.h to record all tests
    //测试启动，打印基本说明
    record_start_test_fputs(str_testCase,file_testCase);
 
    pid_t pc,pr;
    int status=-1;
    int ret;

    pc = fork();
    if(pc==-1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
   
    if(pc==0)
    {
       //创建基本测试值
        
          ret = fputs(generateCSTR(str_testCase.id),generateFILE(file_testCase.id)); 
          exit(ret);
    }
    else
    {
        sleep(wait_time);
        //do{
            pr = waitpid(pc,&status,WUNTRACED|WNOHANG);
            if (pr == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            if(pr==0)
            {
               // kill(pc,SIGKILL);
               record_timedout_test_fputs();

            }
             else if(WIFEXITED(status))   
             {
                 record_ok_test_fputs(WEXITSTATUS(status));
             }
             else if(WIFSIGNALED(status))
             {
               record_crashed_test_fputs(WTERMSIG(status));
             }
             else if(WIFSTOPPED(status))
             {
                record_stopped_test_fputs(WSTOPSIG(status));
             }
             else if(WIFCONTINUED(status))
             {
                 record_timedout_test_fputs();
             }
       // }while(!WIFEXITED(status) && !WIFSIGNALED(status));
       // exit(EXIT_SUCCESS);
    }
    
}

int main(int argc, const char **argv) {
    // execute all tests and catch exceptions
    
    for(int i=0;i<testCases_CSTR_count;i++)
    {
        for(int j=0;j<testCases_FILE_count;j++)
        {
          test_fputs (testCases_CSTR[i],testCases_FILE[j]);
        }
    }
    print_summary();
    return 0;
}

