
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    write(STDOUT_FILENO, "Caught SIGSEGV\n", 15);
    _exit(0);
}

int main() {
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("Generating SIGSEGV...\n");
    fflush(stdout);

    raise(SIGSEGV);

    return 0;
}

