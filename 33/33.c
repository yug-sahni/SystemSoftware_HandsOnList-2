#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int p1[2], p2[2];
    char buffer[100];
    int n;
    pid_t pid;

    if (pipe(p1) == -1 || pipe(p2) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid == 0)
    {
        close(p1[1]);
        close(p2[0]);

        n = read(p1[0], buffer, sizeof(buffer) - 1);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("Child received: %s", buffer);
        }

        printf("Child: Enter reply: ");
        fflush(stdout);
        n = read(0, buffer, sizeof(buffer));

        if (n > 0)
            write(p2[1], buffer, n);

        close(p1[0]);
        close(p2[1]);
    }
    else if (pid > 0)
    {
        close(p1[0]);
        close(p2[1]);

        printf("Parent: Enter message: ");
        fflush(stdout);
        n = read(0, buffer, sizeof(buffer));

        if (n > 0)
            write(p1[1], buffer, n);

        n = read(p2[0], buffer, sizeof(buffer) - 1);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("Parent received: %s", buffer);
        }

        close(p1[1]);
        close(p2[0]);

        wait(NULL);
    }
    else
    {
        perror("fork");
        return 1;
    }

    return 0;
}
