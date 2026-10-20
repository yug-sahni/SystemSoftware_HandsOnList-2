
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

volatile sig_atomic_t expired = 0;

void handler(int sig) {
    expired = 1;
}

int main() {
    struct itimerval timer;

    signal(SIGPROF, handler);

    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;

    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    if (setitimer(ITIMER_PROF, &timer, NULL) == -1) {
        perror("setitimer");
        return 1;
    }

    printf("ITIMER_PROF started. Using CPU...\n");

    while (!expired) {
        // Consume CPU time
    }

    printf("ITIMER_PROF expired!\n");
    return 0;
}

