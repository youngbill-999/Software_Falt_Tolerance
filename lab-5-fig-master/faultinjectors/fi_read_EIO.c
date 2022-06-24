// TODO: read wrapper
#define _GNU_SOURCE

#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

typedef ssize_t (*read_type) (int, void*, size_t);
ssize_t read(int fd, void *buf, size_t count){
    static read_type orig = 0;
    if (drand48() < 0.1){
        errno = EIO;
        return -1;
    }
    if ( 0 == orig){
        orig = (read_type) dlsym(RTLD_NEXT, "read");
        assert(orig && "original read function not found");
    }
    return orig(fd, buf, count);
}