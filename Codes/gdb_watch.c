#include <stdio.h>
#include <unistd.h>

int global_var = 0;

void modify_variable() {
    for (int i = 0; i < 5; i++) {
        global_var += i; // Modify global_var
        sleep(1);        // Simulate delay
    }
}

int main() {
    printf("Initial global_var: %d\n", global_var);
    modify_variable();
    printf("Final global_var: %d\n", global_var);
    return 0;
}
// gcc -g -o gdb_watch gdb_watch.c
