
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

void handler(int sig) {
    write(1, "Caught SIGALRM using setitimer\n", 31);
    _exit(0);
}

int main() {
    struct itimerval timer = {0};

    signal(SIGALRM, handler);

    timer.it_value.tv_sec = 5;

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        return 1;
    }

    printf("ITIMER_REAL set for 5 seconds...\n");
    fflush(stdout);

    while (1)
        pause();

    return 0;
}

