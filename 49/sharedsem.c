#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <string.h>

union semun
{
    int val;
};

int main()
{
    key_t key = ftok("/tmp", 'M');
    int shmid, semid;
    char *ptr;
    union semun arg;

    if (key == -1) { perror("ftok"); return 1; }

    shmid = shmget(key, 1024, IPC_CREAT | 0666);
    if (shmid == -1) { perror("shmget"); return 1; }

    ptr = (char *)shmat(shmid, NULL, 0);
    if (ptr == (char *)-1) { perror("shmat"); return 1; }

    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget");
        shmdt(ptr);
        return 1;
    }

    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("semctl");
        shmdt(ptr);
        return 1;
    }

    for (int i = 1; i <= 2; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            struct sembuf lock = {0, -1, SEM_UNDO};
            struct sembuf unlock = {0, 1, SEM_UNDO};

            if (semop(semid, &lock, 1) == -1)
                _exit(1);

            snprintf(ptr, 1024, "Written by child %d", i);
            printf("Child %d wrote to shared memory.\n", i);

            sleep(1);

            if (semop(semid, &unlock, 1) == -1)
                _exit(1);

            _exit(0);
        }

        if (pid < 0)
        {
            perror("fork");
            break;
        }
    }

    wait(NULL);
    wait(NULL);

    printf("Final shared data: %s\n", ptr);

    shmdt(ptr);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
