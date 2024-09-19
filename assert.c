#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

void* thread_function(void* arg) {
    int id = *(int*)arg;
    
    // Simulate a thread running some native code
    printf("Thread %d is running native code...\n", id);
    usleep(1000);  // This could be considered as native execution

    // Simulate returning to DynamoRIO-controlled code
    printf("Thread %d returning to instrumented code...\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Create multiple threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Simulate detaching DynamoRIO
    printf("Detaching DynamoRIO...\n");
    sleep(2);  // Allow threads to run and potentially enter native execution

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
