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

        dup2(fd[1], 1);
        close(fd[1]);

        execlp("ls", "ls", "-l", NULL);
        perror("execlp");
    }
    else
    {
        close(fd[1]);

        dup2(fd[0], 0);
        close(fd[0]);

        execlp("wc", "wc", NULL);
        perror("execlp");
    }

    return 1;
}
