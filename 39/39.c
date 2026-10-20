#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>

int main()
{
    int fd, result, n;
    char buffer[100];
    fd_set readfds;
    struct timeval timeout;

    mkfifo("myfifo", 0666);

    fd = open("myfifo", O_RDONLY | O_NONBLOCK);
    if (fd == -1)              /* suggested addition */
    {
        perror("open");
        return 1;
    }

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    printf("Waiting for FIFO data for 10 seconds...\n");

    result = select(fd + 1, &readfds, NULL, NULL, &timeout);

    if (result == -1)
    {
        perror("select");
    }
    else if (result == 0)
    {
        printf("Timeout! No data received.\n");
    }
    else if (FD_ISSET(fd, &readfds))
    {
        n = read(fd, buffer, sizeof(buffer) - 1);

        if (n > 0)
        {
            buffer[n] = '\0';
            printf("Received: %s", buffer);
        }
        else
        {
            printf("FIFO became readable, but no data was received.\n");
        }
    }

    close(fd);
    return 0;
}

/*
 in another terminal run
 echo "Hello FIFO" > myfifo

 also try in terminal directly
 cat myfifo
 */
