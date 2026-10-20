#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    int fd, n;
    char buffer[100];

    mkfifo("myfifo", 0666);

    fd = open("myfifo", O_WRONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    printf("Enter message: ");
    fflush(stdout);

    n = read(0, buffer, sizeof(buffer));

    if (n > 0)
        write(fd, buffer, n);

    close(fd);

    return 0;
}
