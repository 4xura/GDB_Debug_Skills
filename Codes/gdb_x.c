#include <stdio.h>
#include <string.h>

int global_var = 0x12345678;
char global_str[] = "Hello, GDB!";
float global_float = 3.14;

int main() {
    int local_var = 0xdeadbeef;
    printf("Debug me!\n");
    return 0;
}
// gcc -g -o gdb_x gdb_x.c
