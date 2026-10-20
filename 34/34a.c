#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    pipe(fd);

    if (fork() == 0)
    {
        close(fd[0]);
        close(1);
        dup(fd[1]);
        close(fd[1]);

        execlp("ls", "ls", "-l", NULL);
        perror("execlp");
    }
    else
    {
        close(fd[1]);
        close(0);
        dup(fd[0]);
        close(fd[0]);

        execlp("wc", "wc", NULL);
        perror("execlp");
    }

    return 1;
}
