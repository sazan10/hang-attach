include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dynamorio.h>

// Number of threads to create
#define NUM_THREADS 10

// Flag to control thread execution
volatile sig_atomic_t running = 1;

// Signal handler for PROF signals
void prof_signal_handler(int signum) {
    printf("Received PROF signal: %d\n", signum);
    // Attempt to read TLS magic (simulating safe_read_tls_magic)
    // This will crash if TLS has been removed
    void *tls = dr_get_tls_field();
    if (tls == NULL) {
        printf("TLS is NULL in signal handler!\n");
        // Force a crash
        *(int*)0 = 0;
    }
}

// Thread function
void* thread_func(void* arg) {
    int thread_id = *((int*)arg);
    printf("Thread %d started.\n", thread_id);
    
    while (running) {
        // Simulate work
        sleep(1);
    }
    
    printf("Thread %d exiting.\n", thread_id);
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    struct sigaction sa;

    // Install signal handler for PROF signals
    sa.sa_handler = prof_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Let threads run for a while
    sleep(5);

    // Send PROF signals asynchronously
    printf("Sending PROF signals...\n");
    for (int i = 0; i < 5; i++) {
        kill(getpid(), SIGUSR1);
        sleep(1);
    }

    // Initiate DynamoRIO detachment
    printf("Initiating DynamoRIO detachment...\n");
    dr_exit_process();

    // Wait for threads to finish
    running = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Application exiting.\n");
    return 0;
}
