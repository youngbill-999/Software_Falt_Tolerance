/// File:
/// A fault injector that randomly aborts the program

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <netdb.h>

#include <dlfcn.h>
#include <sys/types.h>
#include <sys/socket.h>

const double crash_threshold = 0.025;

// Do not use the library constructor function because if drand48
// is not called before the fork, all children will starts
// with the same seed value
static void init() {
    struct timespec now;

    clock_gettime(CLOCK_REALTIME, &now);
    seed48((unsigned short *) &now);
}

static bool should_abort() {
    static bool first = true;
    if (first) {
        first = false;
        init();
    }
    return drand48() < crash_threshold;
}

#define resolver(func)                                                                \
static void resolve_##func();                                                      \
__typeof(func)* libc_##func = NULL; 				                    \
static void __attribute__((constructor))resolve_##func()                                                       \
{                                                                                     \
    if (!libc_##func || (__typeof(func)*)resolve_##func == libc_##func)            \
    {                                                                                 \
        libc_##func = (__typeof(func)*)dlsym (RTLD_NEXT, #func);                      \
        if (NULL == libc_##func)                                                      \
        {                                                                             \
            perror ("Could not find original " #func " function");                    \
            abort ();                                                                 \
        }                                                                             \
    }                                                                                 \
}

resolver(socket)
int socket(int domain, int type, int protocol) {
    if (should_abort())
        abort();
    return libc_socket(domain, type, protocol);
}

resolver(recv)
ssize_t recv(int s, void *buf, size_t len, int flags) {
    if (should_abort())
        abort();
    return libc_recv(s, buf, len, flags); 
}

resolver(gethostbyname)
struct hostent *gethostbyname(const char *name) {
    if (should_abort())
        abort();
    return libc_gethostbyname(name);
}
