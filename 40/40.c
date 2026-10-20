#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <fcntl.h>

int main()
{
    struct rlimit limit;
    int fd[2];
    int size;

    if (getrlimit(RLIMIT_NOFILE, &limit) == -1)
    {
        perror("getrlimit");
        return 1;
    }

    printf("Maximum open file descriptors: ");

    if (limit.rlim_cur == RLIM_INFINITY)
        printf("Unlimited (soft limit)\n");
    else
        printf("%ld (soft limit)\n", (long)limit.rlim_cur);

    printf("Maximum allowed file descriptors: ");

    if (limit.rlim_max == RLIM_INFINITY)
        printf("Unlimited (hard limit)\n");
    else
        printf("%ld (hard limit)\n", (long)limit.rlim_max);

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return 1;
    }

    size = fcntl(fd[0], F_GETPIPE_SZ);

    if (size == -1)
        perror("F_GETPIPE_SZ");
    else
        printf("Pipe capacity: %d bytes\n", size);

    close(fd[0]);
    close(fd[1]);

    return 0;
}
