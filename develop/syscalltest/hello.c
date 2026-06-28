/*
 * From 5.2:
 *
 * Compile the following program with gcc -Wall -o hello hello.c.
 *
 * Run the executable with strace ./hello.
 *
 * Are you impressed? Every line you see corresponds to a system call.
 * strace is a handy program that gives you details about what system calls a program is making,
 * including which call is made, what its arguments are and what it returns.
 * It is an invaluable tool for figuring out things like what files a program is trying to access.
 */

#include <stdio.h>

int main(void)
{
    printf("hello\n");
    return 0;
}