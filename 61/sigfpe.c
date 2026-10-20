
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    write(STDOUT_FILENO, "Caught SIGFPE\n", 14);
    _exit(0);
}

int main() {
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGFPE, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("Generating SIGFPE...\n");
    fflush(stdout);

    raise(SIGFPE);

    return 0;
}

