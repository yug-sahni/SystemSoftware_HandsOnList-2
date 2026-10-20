#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun
{
    int val;
};

int main()
{
    key_t key = ftok("/tmp", 'T');
    int semid;
    union semun arg;

    if (key == -1) { perror("ftok"); return 1; }

    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) { perror("semget"); return 1; }

    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) { perror("semctl"); return 1; }

    printf("Ticket semaphore initialized to 1.\n");
    return 0;
}
