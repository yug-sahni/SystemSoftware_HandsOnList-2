#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>     /* added: needed for wait() */

int main()
{
    int fd[2];
    char buffer[100];
    int n;

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        close(fd[1]);

        n = read(fd[0], buffer, sizeof(buffer) - 1);

        if (n > 0)
        {
            buffer[n] = '\0';
            printf("Child received: %s", buffer);
        }

        close(fd[0]);
    }
    else
    {
        close(fd[0]);

        printf("Parent: Enter a message: ");
        fflush(stdout);

        n = read(0, buffer, sizeof(buffer));

        if (n > 0)
            write(fd[1], buffer, n);

        close(fd[1]);

        wait(NULL);
    }

    return 0;
}
