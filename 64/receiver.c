#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("SIGSTOP caught!\n");
}

int main() {
    struct sigaction sa = {0};

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGSTOP, &sa, NULL) == -1) {
        perror("sigaction");
        printf("SIGSTOP cannot be caught or handled.\n");
    }

    printf("Receiver PID: %d\n", getpid());
    printf("Waiting for a signal...\n");
    fflush(stdout);

    while (1) {
        pause();
    }

    return 0;
}
