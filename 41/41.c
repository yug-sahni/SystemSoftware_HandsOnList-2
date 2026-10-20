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

    msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid == -1)
    {
        perror("msgget");
        return 1;
    }

    printf("Message Queue Key: %d\n", (int)key);
    printf("Message Queue ID: %d\n", msgid);

    return 0;
}

/*
for terminal
ipcs -q
 */
