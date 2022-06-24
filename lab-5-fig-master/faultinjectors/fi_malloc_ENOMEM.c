#define _GNU_SOURCE

#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

typedef void * (*malloc_type) (size_t);
void * malloc (size_t size){
    static malloc_type orig = 0;
    if (drand48() < 0.1){
        errno = ENOMEM;
        return NULL;
    }
    if ( 0 == orig){
        orig = (malloc_type) dlsym(RTLD_NEXT, "malloc");
        assert(orig && "original malloc function not found");
    }
    return orig(size);
}
