#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("Child PID: %d\n", getpid());
        printf("Parent PID before killing: %d\n", getppid());
        fflush(stdout);

        if (kill(getppid(), SIGKILL) == -1) {
            perror("kill");
            return 1;
        }

        sleep(2);

        printf("Child's new parent PID: %d\n", getppid());
        printf("Child is now an orphan.\n");
    } else {
        printf("Parent PID: %d\n", getpid());
        fflush(stdout);

        while (1) {
            pause();
        }
    }

    return 0;
}
