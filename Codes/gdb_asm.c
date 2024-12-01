#include <stdio.h>

int add_numbers(int a, int b) {
    int result;
    __asm__(
        "movl %1, %%eax;" // Move 'a' into EAX
        "addl %2, %%eax;" // Add 'b' to EAX
        "movl %%eax, %0;" // Move EAX to 'result'
        : "=r"(result)    // Output operand
        : "r"(a), "r"(b)  // Input operands
        : "%eax"          // Clobbered registers
    );
    return result;
}

int main() {
    int a = 5, b = 10;
    int result = add_numbers(a, b);
    printf("Result: %d\n", result);
    return 0;
}
// gcc -g -o gdb_asm gdb_asm.c
