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
    key_t key = ftok("/tmp", 'C');
    int semid;
    union semun arg;

    if (key == -1) { perror("ftok"); return 1; }

    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) { perror("semget"); return 1; }

    arg.val = 2;
    if (semctl(semid, 0, SETVAL, arg) == -1) { perror("semctl"); return 1; }

    for (int i = 1; i <= 4; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            struct sembuf P = {0, -1, SEM_UNDO};
            struct sembuf V = {0, 1, SEM_UNDO};

            if (semop(semid, &P, 1) == -1)
                _exit(1);

            printf("Process %d acquired a resource.\n", i);
            fflush(stdout);

            sleep(2);

            printf("Process %d released a resource.\n", i);
            fflush(stdout);

            semop(semid, &V, 1);
            _exit(0);
        }

        if (pid < 0)
        {
            perror("fork");
            break;
        }
    }

    for (int i = 0; i < 4; i++)
        wait(NULL);

    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("IPC_RMID");
        return 1;
    }

    printf("Counting semaphore removed.\n");
    return 0;
}
