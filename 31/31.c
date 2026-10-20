#include <stdio.h>
#include <unistd.h>

int main()
{
    int fd[2];
    char buffer[100];
    int n;

    if (pipe(fd) == -1)
    {
        perror("pipe failed");
        return 1;
    }

    write(fd[1], "Hello through pipe", 18);

    n = read(fd[0], buffer, sizeof(buffer) - 1);

    if (n > 0)
    {
        buffer[n] = '\0';
        printf("Data received: %s\n", buffer);
    }

    close(fd[0]);
    close(fd[1]);

    return 0;
}
