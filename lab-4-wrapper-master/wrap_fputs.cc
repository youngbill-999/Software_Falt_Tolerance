#include <dlfcn.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <list>
using namespace std;
// obtain a list of currently open file handles by wrapping:
// fopen
// fdopen(int fildes,const char * mode)取一个int型的文件描述词，转换成mode类型的FILE文件输出
// freopen(name,mode,FILE)将当前的FILE重新指定到另一个文件变量
// fclose/Users/ybbb/Desktop/sft实验/lab4/Makefile

static list<FILE *> open_file_handles;
FILE *fopen(const char *filename, const char *mode) {
    FILE *(*wrapper_fopen)(const char *, const char *);
    wrapper_fopen = (FILE * (*)(const char *, const char *)) dlsym(RTLD_NEXT, "fopen");
    FILE *file = wrapper_fopen(filename, mode);
    open_file_handles.push_back(file);
    // return wrapper_fopen(filename,mode);
    return file;
}

FILE *fdopen(int fildes, const char *mode) {
    FILE *(*wrapper_fdopen)(int, const char *);
    wrapper_fdopen = (FILE * (*)(int, const char *)) dlsym(RTLD_NEXT, "fdopen");
    FILE *file = wrapper_fdopen(fildes, mode);
    open_file_handles.push_back(file);
    // return wrapper_fdopen(fildes,mode);
    return file;
}
FILE *freopen(const char *filename, const char *mode, FILE *stream) {
    FILE *(*wrapper_freopen)(const char *, const char *, FILE *);
    wrapper_freopen = (FILE * (*)(const char *, const char *, FILE *)) dlsym(RTLD_NEXT, "freopen");
    FILE *file = wrapper_freopen(filename, mode, stream);
    open_file_handles.push_back(file);
    return file;
}
int fclose(FILE *fp) {
    int (*wrapper_fclose)(FILE *);
    wrapper_fclose = (int (*)(FILE *))dlsym(RTLD_NEXT, "fclose");
    open_file_handles.remove(fp);
    return wrapper_fclose(fp);
}


jmp_buf set0;
void signalhandler0(int signum)
{
    longjmp(set0,1);
}
// check for readability of a string (using setjump/longjmp/signal approach)
static bool check_read_str(const char *s) {
    signal(SIGSEGV, signalhandler0);

    int r = setjmp(set0);
    if (r == 0) {
        int len = strlen(s);
        return true;
    } else {
        return false;
    }
}


// check read-/writeability of a file handle (using setjump/longjmp/signal approach)
jmp_buf set;
void signalhandler(int signum)
{
    longjmp(set,1);
}
static bool check_readwrite_FILE(FILE *f) {
    signal(SIGSEGV, signalhandler);
    if (setjmp(set) == 0) {
        int fd = fileno(f);
        int mode = fcntl(fd, F_GETFL);
        if (mode == O_RDONLY) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}



static bool check_FILE(FILE *f) {
    int i = 0;
    list<FILE *>::iterator iter;
    for (iter = open_file_handles.begin(); iter != open_file_handles.end(); iter++) {
        if (*iter == f) {
            return true;
        }
    }
    return false;
}
// WRAP fputs to behave robust
int fputs(const char *s, FILE *f) {
    int (*Func_fputs)(const char *, FILE *);
    Func_fputs = (int (*)(const char *, FILE *))dlsym(RTLD_NEXT, "fputs");
    if (check_FILE(f) && check_readwrite_FILE(f) && check_read_str(s)) {
        return Func_fputs(s, f);
    } else {
        return EOF;
    }
}
