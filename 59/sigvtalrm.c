
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

volatile sig_atomic_t caught = 0;

void handler(int sig) {
    caught = 1;
}

int main() {
    struct itimerval timer = {0};

    signal(SIGVTALRM, handler);

    timer.it_value.tv_sec = 5;

    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
        perror("setitimer");
        return 1;
    }

    printf("ITIMER_VIRTUAL set for 5 seconds of CPU time...\n");

    while (!caught) {
        // Consume user-mode CPU time
    }

    printf("Caught SIGVTALRM\n");
    return 0;
}

