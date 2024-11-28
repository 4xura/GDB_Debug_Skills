#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Global variables
int global_var = 0x12345678;
char global_str[] = "Hello, GDB!";
float global_float = 3.14;

void test_function() {
    int local_var = 0xdeadbeef;         // Local variable
    char local_str[] = "Local String"; // Local string
    int *dynamic_var = malloc(sizeof(int)); // Dynamic memory allocation

    *dynamic_var = 0xfeedface;

    printf("Inside test_function:\n");
    printf("local_var: %x\n", local_var);
    printf("local_str: %s\n", local_str);
    printf("dynamic_var: %x\n", *dynamic_var);

    free(dynamic_var); // Free allocated memory
}

int main() {
    printf("Start debugging with GDB!\n");

    // Check the global variables
    printf("global_var: %x\n", global_var);
    printf("global_str: %s\n", global_str);
    printf("global_float: %.2f\n", global_float);

    // Call test_function to inspect local and dynamically allocated variables
    test_function();

    printf("End of program.\n");
    return 0;
}
// gcc -g -o gdb_x gdb_x.c
