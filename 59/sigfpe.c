
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    write(1, "Caught SIGFPE\n", 14);
    _exit(0);
}

int main() {
    signal(SIGFPE, handler);

    printf("Generating SIGFPE...\n");
    fflush(stdout);

    raise(SIGFPE);

    return 0;
}

