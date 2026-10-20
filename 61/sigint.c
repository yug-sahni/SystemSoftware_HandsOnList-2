#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    write(STDOUT_FILENO, "\nCaught SIGINT\n", 13);
}

int main() {
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("Press Ctrl+C to generate SIGINT.\n");

    while (1)
        pause();

    return 0;
}
