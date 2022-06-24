# Exceptions in Java and C++

Exceptions are central to the C++ as well as the Java programming paradigm.
They provide a powerful structured mechanism for dealing with errors that is far more flexible than the traditional Unix/C model of using a function's return value to indicate success or failure.

From Java tutorial:

> The Java programming language uses exceptions to provide error-handling capabilities for its programs.
> An exception is an event that occurs during the execution of a
program that disrupts the normal flow of instructions.
> 
> When an error occurs within a method, the method creates an object and hands it off to the runtime system.
> The object, called an exception object, contains information about the error, including its type and the state of the program when the error occurred. 
> Creating an exception object and handing it to the runtime system is called throwing an exception. 
> 
>After a method throws an exception, [...] the runtime system searches the call stack for a method that contains a block of code that can handle the exception.
>This block of code is called an exception handler.
> [...] The exception handler chosen is said to catch the
exception.
> 
> If no exception handler is found the program terminates.

For further information, see:
https://docs.oracle.com/javase/tutorial/essential/exceptions/

# Background

## Basic Components

* `try`: block of statements, where exceptions can occur and be caught
* `catch`: block of statements to handle exceptions, that occurred in corresponding try block
* `throw`: keyword to throw exception

## Exceptions in Java

In Java exceptions are instances of corresponding exception classes and can inherit from
`java.lang.Exception` (must at least extend `java.lang.Throwable`).

"Non-runtime-exceptions" are part of methods signature.

```java
void foo (...) throws IOException, FileNotFoundException
```

In Java there are, unlike C++, no (real) destructors for cleaning up.

### Java – finally

`finally`: for cleaning up after exception may have occurred

```java
FileInputStream f = new FileInputStream (filename);
try {
    // do something with f
    throw new Exception ();
} finally {
    f.close ();
}
```

`finally` is always executed even if exception has occurred.

### Java – by Example

```java
import java.io.*;
public class Example {
    private static RandomAccessFile file;
    private static String test;
    public static void main(String[] args) {
        try {
            file = new RandomAccessFile(new
            File("./textfile"),"r");
            try {
                test = file.readUTF();
            } finally {
                file.close();
            }
        } catch(FileNotFoundException fnfe) {
            System.out.println("File not found.");
        } catch(EOFException e1) {
            System.out.println("Read failed: End of file has been reached.");
        } catch(UTFDataFormatException e2) {
            System.out.println("Read failed: wrong data format.");
        } catch(IOException e3) {
            System.out.println("Read failed: I/O problem.");
        } catch(Exception e) {
            System.out.println("Read failed: unknown Problem.");
        }
    }
}
```

It is important to note that the order of catch-statements is crucial. The most general exception must be caught at last.

## Exceptions in C++

In C++ an exception object can be any type (also int, float and so on...).

```c++
try{
    // do something
    throw 10;
} catch (int i){
    cout << "caught " << i << endl;
}
```

In C++ exceptions should be caught by reference, except for basic types like int.

### Resource Acquisition Is Initialization (RAII)

C++ has no `finally` clause like Java. But you can use the destructor to free resources even in the presence of exceptions. That technique is called "resource acquisition is initialization" (RAII) or scoped objects.

```c++
class File {
 public:
    File (...) {
        f = fopen (...);
        ...
    }
    ~File() {
        if (NULL != f)
            fclose (f);
    }
};
...
try {
    File file (...);
    // do something
        // destructor of file will be called
    // and file will be closed
} catch (...) {
    ...
}
```

Object file is called scoped object, because it lives only in the scope of `try` (between the brackets).
The compiler ensures that if the execution leaves a scope all destructors of objects that live only in this scope are executed.
A scope can be left normal control flow or by an exception.
That's why scoped objects are a nice way to wrap resources in C++.
You get the clean up nearly for free.

Note: Dynamic allocated objects (allocated by `new`) are not scoped objects!
They live on the heap and their lifetime is not bounded by any scope.
The destructor of a dynamic object is only by an explicit `delete`.

# Task Description

Today's tasks are about exceptions in C++ and Java. You have to improve the robustness of your `rcopy`. The second part is about finding an exception-related bug in Java.

## Task 1: Exceptions in rcopy (C/C++)

Where and how can you use exceptions in your rcopy application? Extend the C Version of rcopy with exceptions (of course now in C++). Do not reuse your C++ version from task 1. Write C++ wrapper classes for your C solution! Help: http://www.cplusplus.com/doc/tutorial/ 

Details:
* wrapper class for File with:
    * `open`, `seek`, `read`, `write`, `close`, `size`...
    * destructor should call `close` if necessary
* wrapper class for memory realm: `Buffer`
    * constructor allocates: `Buffer b (1024);`
    * method for reversing the buffers content
    * destructor de-allocates memory
    * class `File` has to work together with class `Buffer`
    
Also take care of exception handling. You should at least catch exceptions and provide some error message. For doing so you should replace the standard error codes:

```c++
#include <cerrno>

class Exception {
    int error_code;
    ...
    private:
        Exception (int e): error_code (e) {}
    ...
};
...
if (-1 == fseek (...))
    throw Exception (errno);
```

Do not forget cleaning up and freeing resources with the help of scoped objects.

```c++
File f;
try {
    f.open (...);
    // do something
} catch (const Exception& e){
    // do some recovery
} // f.~File () is called for local
// variables if their block is done
// no matter if an exceptions is
// thrown or not
```

Make your rcopy robust against all kinds of user or operating system errors. That means – check all return values for operating system errors (mostly I/O errors) and command line arguments for user errors.

Stick to our files and interfaces that we provide in the repo. You may add methods (e.g. for file size or reversing the buffers content), but you are not allowed to change the signature of any method specified by us.

## Task 2: Exceptions in MyStack (Java)

You shall find the bug in the class MyStack.

Capabilities of MyStack are:
* storing arbitrary objects on a stack
* reallocating memory if necessary
* returning the number of elements on the stack
* returning a string representation of the stack contents

You should implement your own main method. Find the included bug by testing the methods of MyStack. The bug may cause an error, that leaves the MyStack object in an inconsistent state. Find and fix the bug the implementation!

Hint: Think about invariants of our stack implementation and how to break them with the existing methods. Keep in mind: the current task is about exceptions. So the bug may be related to exceptions somehow.

Please mind that there is no method to get the last element or remove elements from the stack. This is NOT the bug. It is just to keep the example as simple as possible.

# Hints

Here are a few hints for task 2:
* look for RAII in your favorite C++ book (e.g. Scott Meyers: “More Effective C++” or on the Internet)
* destructor must not throw exceptions
* check the return values of function calls – do not solely rely on ferror, except for
fread. Using ferror is usually fine, but you might fail our fault injection tests.
* the function stat the file size among others
* throw an exception if your API is miss-used (e.g. reading from an already closed file)
* prevent undefined behavior (see fclose)
