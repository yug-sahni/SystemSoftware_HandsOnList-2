#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    signal(SIGINT, SIG_IGN);

    printf("SIGINT is ignored. Press Ctrl+C now.\n");
    sleep(10);

    signal(SIGINT, SIG_DFL);

    printf("SIGINT reset to default. Press Ctrl+C now.\n");

    while (1) {
        pause();
    }

    return 0;
}
