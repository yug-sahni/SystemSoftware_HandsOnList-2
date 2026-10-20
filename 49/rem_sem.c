#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{
    key_t key = ftok("/tmp", 'C');
    int semid;

    if (key == -1) { perror("ftok"); return 1; }

    semid = semget(key, 1, 0666);
    if (semid == -1) { perror("semget"); return 1; }

    if (semctl(semid, 0, IPC_RMID) == -1) { perror("semctl"); return 1; }

    printf("Semaphore removed successfully.\n");
    return 0;
}
