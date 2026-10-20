#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

union semun
{
    int val;
};

int main()
{
    int semid;
    key_t key = ftok("/tmp", 'D');
    union semun arg;

    semid = semget(key, 2, IPC_CREAT | 0666);

    if (semid == -1)
    {
        perror("semget");
        return 1;
    }

    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);
    semctl(semid, 1, SETVAL, arg);

    if (fork() == 0)
    {
        struct sembuf lock1 = {0, -1, 0};
        struct sembuf lock2 = {1, -1, 0};

        semop(semid, &lock1, 1);
        printf("Process 1 acquired Semaphore 1\n");
        fflush(stdout);

        sleep(1);

        printf("Process 1 waiting for Semaphore 2...\n");
        fflush(stdout);
        semop(semid, &lock2, 1);

        _exit(0);
    }
    else
    {
        struct sembuf lock2 = {1, -1, 0};
        struct sembuf lock1 = {0, -1, 0};

        semop(semid, &lock2, 1);
        printf("Process 2 acquired Semaphore 2\n");
        fflush(stdout);

        sleep(1);

        printf("Process 2 waiting for Semaphore 1...\n");
        fflush(stdout);
        semop(semid, &lock1, 1);

        wait(NULL);
    }

    return 0;
}
