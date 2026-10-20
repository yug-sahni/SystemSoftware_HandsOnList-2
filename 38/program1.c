#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    int fd1, fd2, n;
    char buffer[100];

    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    fd1 = open("fifo1", O_WRONLY);
    fd2 = open("fifo2", O_RDONLY);

    printf("Program 1 - Enter message: ");
    fflush(stdout);
    n = read(0, buffer, sizeof(buffer));

    if (n > 0)
        write(fd1, buffer, n);

    n = read(fd2, buffer, sizeof(buffer) - 1);

    if (n > 0)
    {
        buffer[n] = '\0';
        printf("Reply from Program 2: %s", buffer);
    }

    close(fd1);
    close(fd2);

    return 0;
}
