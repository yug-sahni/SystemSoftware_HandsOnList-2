#include <sys/ipc.h>
#include <sys/sem.h>

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void P(int semid)
{
    struct sembuf op = {0, -1, SEM_UNDO};
    semop(semid, &op, 1);
}

void V(int semid)
{
    struct sembuf op = {0, 1, SEM_UNDO};
    semop(semid, &op, 1);
}
