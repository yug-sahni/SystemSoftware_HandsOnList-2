
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    write(1, "Caught SIGSEGV\n", 15);
    _exit(0);
}

int main() {
    signal(SIGSEGV, handler);

    printf("Generating SIGSEGV...\n");
    fflush(stdout);

    raise(SIGSEGV);

    return 0;
}

