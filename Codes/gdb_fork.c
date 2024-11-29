#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_var = 10;

int main() {
    printf("Parent process: PID = %d\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        printf("Child process: PID = %d, Parent PID = %d\n", getpid(), getppid());
        global_var += 5;
        printf("Child process: global_var = %d\n", global_var);
    } else {
        // Parent process
        printf("Parent process: PID = %d, Child PID = %d\n", getpid(), pid);
        global_var += 10;
        printf("Parent process: global_var = %d\n", global_var);
    }

    printf("Process PID = %d exiting.\n", getpid());
    return 0;
}
// gcc -g -o gdb_fork gdb_fork.c
