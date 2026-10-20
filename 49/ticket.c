#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

int main()
{
    key_t key = ftok("/tmp", 'T');
    int semid, fd, ticket;

    if (key == -1)
    {
        perror("ftok");
        return 1;
    }

    semid = semget(key, 1, 0666);   /* attach only — do NOT re-initialize here */
    if (semid == -1)
    {
        perror("semget (did you run init_ticket first?)");
        return 1;
    }

    struct sembuf lock = {0, -1, SEM_UNDO};
    struct sembuf unlock = {0, 1, SEM_UNDO};

    if (semop(semid, &lock, 1) == -1)
    {
        perror("semop lock");
        return 1;
    }

    fd = open("ticket.txt", O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
        perror("open");
        semop(semid, &unlock, 1);
        return 1;
    }

    if (lseek(fd, 0, SEEK_END) == 0)
    {
        ticket = 100;
        write(fd, &ticket, sizeof(ticket));
    }

    lseek(fd, 0, SEEK_SET);
    if (read(fd, &ticket, sizeof(ticket)) != sizeof(ticket))
    {
        perror("read");
        close(fd);
        semop(semid, &unlock, 1);
        return 1;
    }

    ticket++;

    lseek(fd, 0, SEEK_SET);
    if (write(fd, &ticket, sizeof(ticket)) != sizeof(ticket))
    {
        perror("write");
        close(fd);
        semop(semid, &unlock, 1);
        return 1;
    }

    printf("New ticket number: %d\n", ticket);

    close(fd);

    if (semop(semid, &unlock, 1) == -1)
    {
        perror("semop unlock");
        return 1;
    }

    return 0;
}
