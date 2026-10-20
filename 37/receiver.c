#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd, n;
    char buffer[100];

    fd = open("myfifo", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    n = read(fd, buffer, sizeof(buffer) - 1);

    if (n > 0)
    {
        buffer[n] = '\0';
        printf("Received: %s", buffer);
    }

    close(fd);

    return 0;
}
