#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    if (mknod("fifo7", S_IFIFO | 0666, 0) == -1)
    {
        perror("mknod");
        return 1;
    }

    printf("FIFO created using mknod().\n");
    return 0;
}
