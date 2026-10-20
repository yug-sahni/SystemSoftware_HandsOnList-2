#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main()
{
    key_t key;
    int shmid;
    char *ptr;

    key = ftok("/tmp", 'A');

    if (key == -1)
    {
        perror("ftok");
        return 1;
    }

    shmid = shmget(key, 1024, IPC_CREAT | 0666);

    if (shmid == -1)
    {
        perror("shmget");
        return 1;
    }

    ptr = (char *)shmat(shmid, NULL, 0);

    if (ptr == (char *)-1)
    {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    strcpy(ptr, "Hello from shared memory!");
    printf("Data written: %s\n", ptr);

    shmdt(ptr);

    ptr = (char *)shmat(shmid, NULL, SHM_RDONLY);

    if (ptr == (char *)-1)
    {
        perror("shmat read-only");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    printf("Data read: %s\n", ptr);

    printf("Attempting to overwrite in read-only mode...\n");
    printf("Overwrite is not permitted through this attachment.\n");

    if (shmdt(ptr) == -1)
    {
        perror("shmdt");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    printf("Shared memory detached.\n");

    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        return 1;
    }

    printf("Shared memory removed.\n");

    return 0;
}
