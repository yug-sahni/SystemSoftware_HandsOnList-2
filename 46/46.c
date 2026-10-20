#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{
    key_t key;
    int msgid;

    key = ftok("/tmp", 'A');

    if (key == -1)
    {
        perror("ftok");
        return 1;
    }

    msgid = msgget(key, 0666);

    if (msgid == -1)
    {
        perror("msgget");
        return 1;
    }

    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        return 1;
    }

    printf("Message queue removed successfully.\n");

    return 0;
}
