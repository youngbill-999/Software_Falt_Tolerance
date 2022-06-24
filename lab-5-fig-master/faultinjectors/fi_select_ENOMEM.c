// TODO: select wrapper
#define _GNU_SOURCE

#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <sys/select.h>

typedef int (*select_type) (int, fd_set *, fd_set *, fd_set *, struct timeval *);
int select (int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout){
    static select_type orig = 0;
    if (drand48() < 0.1){
        errno = ENOMEM;
        return -1;
    }
    if ( 0 == orig){
        orig = (select_type) dlsym(RTLD_NEXT, "select");
        assert(orig && "original select function not found");
    }
    return orig(nfds, readfds, writefds, exceptfds, timeout);
}