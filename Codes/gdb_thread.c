#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Shared counter
int counter = 0;

// Mutex for synchronization
pthread_mutex_t counter_mutex;

// Worker function for threads
void* worker_function(void* arg) {
    int thread_id = *(int*)arg;

    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&counter_mutex);
        counter++;
        printf("Thread %d incremented counter to %d\n", thread_id, counter);
        pthread_mutex_unlock(&counter_mutex);

        sleep(1); // Simulate work
    }

    return NULL;
}

int main() {
    printf("Starting multithreaded program...\n");

    pthread_t threads[3];
    int thread_ids[3];

    // Initialize mutex
    pthread_mutex_init(&counter_mutex, NULL);

    // Create threads
    for (int i = 0; i < 3; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, worker_function, &thread_ids[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&counter_mutex);
    printf("Final counter value: %d\n", counter);
    return 0;
}
// gcc -g -pthread -o gdb_threads gdb_threads.c
