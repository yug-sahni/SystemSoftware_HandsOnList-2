#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    struct sigaction sa;

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("SIGINT ignored for 10 seconds. Press Ctrl+C.\n");
    sleep(10);

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("SIGINT reset to default. Press Ctrl+C.\n");

    while (1) {
        pause();
    }

    return 0;
}
