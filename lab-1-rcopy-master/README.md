# File Reverse (Introduction)

The purpose of this task is to refresh your knowledge of different programming languages.
To this end, you will have to write a simple program in three languages: C, C++, and Java.

If necessary, you can find help in online references and
API definitions, such as:
* https://docs.oracle.com/javase/8/docs/api/overview-summary.html
* http://www.cppreference.com/index.html
* `$ man <function name>`
* https://stackoverflow.com/

Please read the tasks description carefully. If you have a question that you cannot resolve and you cannot find an answer in the task description, post a message into our auditorium group: https://auditorium.inf.tu-dresden.de/en/groups/2072311

Make yourself familiar with Git and GitLab beside reading the task description. You will have to use it to submit your solutions as well as to test it. And remember to submit your correct solution within the deadline!

# Task

Your task is to write a program that copies a file and reverses its content byte by byte. We request you to implement your application in three different programming
languages: C, C++ and Java. This task is the basis for the following two tasks.

Your C and C++ executable shall be called `rcopy`, so they can be executed with

```shell script
$ rcopy file1 file2
```

In Java we want your file to be named `RCopy`, so execution can be done by

```shell script
$ java -cp . RCopy file1 file2
```

Write a "reverse comparer" (`rdiff`) to test your solutions in C. Test the solution using a large random file created with the unix command line tool `dd`.

Your implementation has to use a buffer of adequate size. Do not read large files completely into
memory, because they may be arbitrary large. Do not read or write files byte-wise, this would be too slow.

## Example usage

```shell script
$ cat file1
0123456789
$ rcopy file1 file2
$ cat file2
9876543210$ rdiff file1 file2
$ java -cp . Rcopy file2 file3
$ cat file3
0123456789
$ diff file1 file3
$ _
```

Note that `file2` starts with an carriage return, because `file1` ends with one. `rdiff` has no output if
both files are "reversed equal" to each other and it has to exit with 0. Otherwise it has to exit with 1
and print an error message to `stdout`.

## Working Environment

For C and C++ your working Environment will be any Linux distribution (Ubuntu, Fedora, Cent OS, Debian, etc.) with
GCC. If you have Windows, you can use `cygwin` (http://www.cygwin.com). We suggest that you use Linux. Alternatively, you could try using Bash for Windows, but this option is not tested and we do not guarantee that everything will work correctly.

For Java you will have to use Java SDK 8+ (https://www.oracle.com/java/technologies/javase/javase-jdk8-downloads.html) or any other Java 8+ environment.

## Requirements

Apart from needing basic skills in using the languages C, C++ and Java you will have to be able to create a build scripts.
For C and C++, you will use GNU Make and for Java - Apache Ant.

In the first task, however, we've already prepared the scripts for you (`Makefile`, `build.xml`). You only have to learn how to use them.

## Hints

Familiarize yourself with the file I/O API of the programming languages you will use.
For more information, see the corresponding documentation.

Useful functions in C:
[`fopen`](https://www.cplusplus.com/reference/cstdio/fopen/), 
[`fclose`](https://www.cplusplus.com/reference/cstdio/fclose/), 
[`fread`](https://www.cplusplus.com/reference/cstdio/fread/), 
[`fwrite`](https://www.cplusplus.com/reference/cstdio/fwrite/), 
[`feof`](https://www.cplusplus.com/reference/cstdio/feof/), 
[`fseek`](https://www.cplusplus.com/reference/cstdio/fseek/), 
[`malloc`](https://www.cplusplus.com/reference/cstdlib/malloc/), 
[`free`](https://www.cplusplus.com/reference/cstdlib/free/).

In C++:
[`fstream`](https://www.cplusplus.com/reference/fstream/fstream/), 
[`ifstream`](https://www.cplusplus.com/reference/fstream/ifstream/), 
[`ofstream`](https://www.cplusplus.com/reference/fstream/ofstream/), 
[`istream::gcount()`](https://www.cplusplus.com/reference/istream/istream/gcount/),
[`iostream::close()`](https://www.cplusplus.com/reference/fstream/fstream/close/),
[`iostream::is_open()`](https://www.cplusplus.com/reference/fstream/fstream/is_open/),
[`iostream::bad()`](https://www.cplusplus.com/reference/ios/ios/bad/),
[`iostream::good()`](https://www.cplusplus.com/reference/ios/ios/good/).

In Java:
[`File`](https://docs.oracle.com/javase/8/docs/api/java/nio/file/Files.html), 
[`RandomAccessFile`](https://docs.oracle.com/javase/8/docs/api/java/io/RandomAccessFile.html), 
[`RandomAccessFile.read`](https://docs.oracle.com/javase/8/docs/api/java/io/RandomAccessFile.html#read--), https://docs.oracle.com/javase/8/docs/api/java/io/FileOutputStream.html#write-byte:A-
[`RandomAccessFile.seek`](https://docs.oracle.com/javase/8/docs/api/java/io/RandomAccessFile.html#seek-long-), 
[`RandomAccessFile.close`](https://docs.oracle.com/javase/8/docs/api/java/io/RandomAccessFile.html#close--), 
[`FileOutputStream`](https://docs.oracle.com/javase/8/docs/api/java/io/FileOutputStream.html), 
[`FileOutputStream.write`](https://docs.oracle.com/javase/8/docs/api/java/io/FileOutputStream.html#write-byte:A-), 
[`FileOutputStream.close`](https://docs.oracle.com/javase/8/docs/api/java/io/FileOutputStream.html#close--).

### Examples

The following two example show how to copy a file without reversing it. You may use them as a starting
point.

#### Example in C

```c
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

int main (int argc, char** argv) {
    void *buffer; FILE *f1, *f2; int read;

    buffer = malloc (BUFFER_SIZE);
    f1 = fopen (argv[1], "r");
    f2 = fopen (argv[2], "w");
    while (!feof (f1)) {
        read = fread (buffer, 1, BUFFER_SIZE, f1);
        fwrite (buffer, 1, read, f2);
    }
    fclose (f1); fclose (f2);
    free (buffer);
    return 0;
}
```

Makefile
```makefile
copy: copy.o
    gcc copy.o -o copy

copy.o: copy.c
    gcc -c copy.c

clean:
    rm copy copy.o
```

Output:
```shell script
$ make
gcc -c copy.c
gcc copy.o -o copy
$ ./copy test.txt test2.txt
coping from test.txt to test2.txt
$ make clean
rm copy copy.o
```

#### Java Example

```java
import java.io.*;
public class Copy {
    public static void main (String[] args) {
        try {
            RandomAccessFile inFile = new RandomAccessFile(args[0], "r");
            FileOutputStream outStream = new FileOutputStream(args[1]);
            byte[] buffer = new byte [1024];
            int bytesRead;
            do {
                bytesRead = inFile.read (buffer);
                if (-1 != bytesRead)
                    outStream.write (buffer, 0, bytesRead);
            } while (-1 != bytesRead);
            inFile.close ();
            outStream.close ();
        } catch (IOException e) {
            System.out.println ("IO-Exception while: " + e.toString());
        }
    }
}
```

# Organizational Remarks

## Deadline

Be aware that there is a deadline for this task.
You can find the exact date published on our website.
If you do not commit your solution until the specified day we will consider your solution missing and the lab failed.

## Directory Structure

We require a specific directory structure for your solutions.
This is a requirement of our automated testing system.
There is no need to create these directories manually, just make sure you do not remove or rename directories in the repository.

```
c/
    Makefile - C Makefile for rcopy
    rcopy.c - C implementation of rcopy
cpp/
    Makefile - C++ Makefile for rcopy
    rcopy.cc - C++ implementation of rcopy
java/
    Rcopy.java - Java implementation of rcopy
    build.xml - Apache Ant build file „make for Java“
rdiff/
    Makefile - Makefile for rdiff
    rdiff.c - C implementation of rdiff
```
