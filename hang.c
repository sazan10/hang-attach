#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 10

void* thread_function(void* arg) {
    printf("Thread %d is running\n", *(int*)arg);
    // Some threads exit quickly
    if (*(int*)arg % 2 == 0) {
        printf("Thread %d is exiting quickly\n", *(int*)arg);
        pthread_exit(NULL);
    }
    // Other threads keep running
    sleep(10);
    printf("Thread %d finished\n", *(int*)arg);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
