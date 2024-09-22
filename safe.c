
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

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

int main() {
    pthread_t threads[4];
    struct sigaction sa;

    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGPROF, &sa, NULL);

    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, thread_func, NULL);
    }

    while (1) {
        // Send SIGPROF signals to threads
        for (int i = 0; i < 4; i++) {
            pthread_kill(threads[i], SIGPROF);
        }
        sleep(1);
    }

    return 0;
}
