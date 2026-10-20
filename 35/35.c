#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int p1[2], p2[2];

    pipe(p1);
    pipe(p2);

    if (fork() == 0)
    {
        /* First command: ls -l */
        dup2(p1[1], 1);

        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);

        execlp("ls", "ls", "-l", NULL);
        perror("ls");
        return 1;
    }

    if (fork() == 0)
    {
        /* Second command: grep ^d */
        dup2(p1[0], 0);
        dup2(p2[1], 1);

        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);

        execlp("grep", "grep", "^d", NULL);
        perror("grep");
        return 1;
    }

    if (fork() == 0)
    {
        /* Third command: wc -l */
        dup2(p2[0], 0);

        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);

        execlp("wc", "wc", "-l", NULL);
        perror("wc");
        return 1;
    }

    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);

    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}
