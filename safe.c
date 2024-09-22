#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void signal_handler(int signo) {
    printf("Signal %d received by thread %ld\n", signo, pthread_self());
}

void* thread_func(void* arg) {
    while (1) {
        // Busy loop to keep thread running
        sleep(1);
    }
    return NULL;
}

void alarm_handler(int signo) {
    printf("Program terminating after timeout.\n");
    exit(0);  // Exit the program cleanly
}

int main() {
    pthread_t threads[4];
    struct sigaction sa;

    // Set up signal handler for SIGPROF
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGPROF, &sa, NULL);

    // Set up signal handler for alarm timeout (SIGALRM)
    struct sigaction alarm_sa;
    alarm_sa.sa_handler = alarm_handler;
    sigemptyset(&alarm_sa.sa_mask);
    alarm_sa.sa_flags = 0;
    sigaction(SIGALRM, &alarm_sa, NULL);

    // Create threads
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, thread_func, NULL);
    }

    // Set an alarm to terminate the program after 10 seconds
    alarm(10);

    // Send signals to threads in a loop
    while (1) {
        for (int i = 0; i < 4; i++) {
            pthread_kill(threads[i], SIGPROF);
        }
        sleep(1);  // Avoid tight looping
    }

    return 0;
}
