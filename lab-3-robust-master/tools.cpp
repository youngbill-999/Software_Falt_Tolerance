/* 
 * The file belongs to task 4 of SFT computer lab course
 * No need to change.
 * For documentation see tools.h
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>

#include <string>

#include "tools.h"

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif

#define MMAP_INSTEAD_MPROTECT

//将文件保存为只读或者只写
void *malloc_prot(size_t s, const void *source, int prot) {
    static const int pagesize = getpagesize();
    void *chunk, *result;

    int pages = (s / pagesize) + (s % pagesize == 0 ? 0 : 1);
    size_t res_size = (pages + 1) * pagesize;
#if defined (__linux__) || defined (__Linux__) || defined (__LINUX__)
    if (PROT_WRITE == prot) {
        chunk = mmap(0, res_size, prot, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        //将一个文件或者其它对象映射进内存
        //成功时返回该内存段的指针
        //void* mmap(void* start,size_t length,int prot,int flags,int fd,off_t offset);
        //start:映射区的开始地址，设置为0时表示由系统决定映射区的起始地址
        //length:映射区的长度。//长度单位是 以字节为单位，不足一内存页按一内存页处理
        //flag:指定映射对象的类型，映射选项和映射页是否可以共享。
        //fd:-1时需要falg中的MAP_ANONYMOUS，表示匿名映射
        //offset：被映射对象内容的起点。
        if ((void *) -1 == chunk)
            return 0;
        result = (char *) chunk + pages * pagesize - s;
        if (0 != munmap((char *) chunk + pages * pagesize, pagesize))
            return 0;

        return result;
    }
#endif // LINUX
    chunk = valloc(res_size);//分配res_size大小的内存
    if (0 == chunk)
        return 0;
    result = (char *) chunk + pages * pagesize - s;
    memcpy(result, source, s);
    if (0 != mprotect(chunk, pages * pagesize, prot))
        return 0;
    if (0 != munmap((char *) chunk + pages * pagesize, pagesize))
        return 0;

    return result;
}
//崩溃的测试可能会破坏文件。这就是为什么您必须在打开测试文件以生成FILE测试值之前复制它。您可以使用filecopy它。
void filecopy(const char *source, const char *dest) {
    const static int PUFFER_SIZE = 1024;
    void *puffer;
    FILE *fsource, *fdest;
    int read;

    puffer = malloc(PUFFER_SIZE);
    if (0 == puffer)
        throw std::string("Insufficient memory to copy a file.");

    fsource = fopen(source, "r");
    fdest = fopen(dest, "w");
    if (0 == fsource || 0 == fdest)
        throw std::string("Couldn't open files to copy.");

    while (!feof(fsource)) {
        read = fread(puffer, 1, PUFFER_SIZE, fsource);
        if (ferror(fsource))
            throw std::string("Error reading source file.");
        if (read != fwrite(puffer, 1, read, fdest))
            throw std::string("Error writing destination file.");
    }

    fclose(fsource);
    fclose(fdest);

    free(puffer);
}

//生成一些测试值可能需要零的存储页
const void *NULLpage() {
    static void *NULL_page = 0;
    if (0 == NULL_page) {
        int size = getpagesize();
        NULL_page = malloc(size);
        if (0 == NULL_page)
            throw std::string("couldn't reserve memory");
        memset(NULL_page, 0, size);//复制字符 c（一个无符号字符）到参数 str 所指向的字符串的前 n 个字符。
    }

    return NULL_page;
}

void sleep(double time) {
    timespec tm;
    tm.tv_sec = (int) (time);
    tm.tv_nsec = (int) ((time - trunc(time)) * 1000000000);
    nanosleep(&tm, 0);
}
