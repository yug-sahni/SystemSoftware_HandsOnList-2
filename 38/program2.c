#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd1, fd2, n;
    char buffer[100];

    fd1 = open("fifo1", O_RDONLY);
    fd2 = open("fifo2", O_WRONLY);

    n = read(fd1, buffer, sizeof(buffer) - 1);

    if (n > 0)
    {
        buffer[n] = '\0';
        printf("Message from Program 1: %s", buffer);
    }

    printf("Program 2 - Enter reply: ");
    fflush(stdout);
    n = read(0, buffer, sizeof(buffer));

    if (n > 0)
        write(fd2, buffer, n);

    close(fd1);
    close(fd2);

    return 0;
}
