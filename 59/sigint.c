#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    write(1, "\nCaught SIGINT\n", 13);
}

int main() {
    signal(SIGINT, handler);

    printf("Press Ctrl+C to generate SIGINT.\n");

    while (1)
        pause();

    return 0;
}
