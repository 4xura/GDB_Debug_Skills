#include <stdio.h>

void buggy_function(int x) {
    int result = 0;
    for (int i = 0; i < 10; i++) {
        result += x / (i - 5); // Intentional division by zero when i == 5
        printf("i: %d, result: %d\n", i, result);
    }
}

int main() {
    buggy_function(100);
    return 0;
}
// gcc -g -o gdb_reverse gdb_reverse.c
