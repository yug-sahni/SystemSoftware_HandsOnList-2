
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    write(1, "Caught SIGALRM\n", 15);
    _exit(0);
}

int main() {
    signal(SIGALRM, handler);

    printf("Alarm set for 5 seconds...\n");
    alarm(5);

    while (1)
        pause();

    return 0;
}

