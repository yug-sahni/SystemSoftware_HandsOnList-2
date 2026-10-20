#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message
{
    long type;
    char text[100];
};

int main(int argc, char *argv[])
{
    key_t key;
    int msgid, flag;
    struct message msg;

    if (argc != 2)
    {
        printf("Usage: %s 0|1\n", argv[0]);
        return 1;
    }

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

    if (argv[1][0] == '0')
        flag = 0;
    else
        flag = IPC_NOWAIT;

    printf("Waiting to receive message...\n");

    if (msgrcv(msgid, &msg, sizeof(msg.text), 0, flag) == -1)
    {
        perror("msgrcv");
        return 1;
    }

    printf("Received message: %s\n", msg.text);

    return 0;
}

/*
 ./44 0 or 1
 */
/*
 *gcc ~/Desktop/HandsOnList-2/41/41.c -o 41run
gcc ~/Desktop/HandsOnList-2/43/43.c -o 43run
gcc ~/Desktop/HandsOnList-2/44/44.c -o 44run
./41run      # create the queue
./43run      # send a message
./44run 0    # receive it — now works, because "." is the same folder every time
Message Queue Key: 1090651803
Message Queue ID: 0
Enter message: hellllllo
Message sent successfully.
Message Queue ID: 0
Waiting to receive message...
Received message: hellllllo
*/
