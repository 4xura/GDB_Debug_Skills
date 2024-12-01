#include <stdio.h>
#include <stdlib.h>

void faulty_function() {
    int *ptr = NULL;
    *ptr = 42; // This will cause a segmentation fault
}

int main() {
    printf("Program is about to crash...\n");
    faulty_function();
    return 0;
}
// gcc -g -o gdb_crash gdb_crash.c
