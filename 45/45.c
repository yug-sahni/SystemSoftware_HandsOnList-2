#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{
    key_t key;
    int msgid;
    struct msqid_ds info;

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

    if (msgctl(msgid, IPC_STAT, &info) == -1)
    {
        perror("IPC_STAT");
        return 1;
    }

    printf("Old permissions: %o\n",
           (unsigned int)(info.msg_perm.mode & 0777));

    info.msg_perm.mode = (info.msg_perm.mode & ~0777) | 0600;

    if (msgctl(msgid, IPC_SET, &info) == -1)
    {
        perror("IPC_SET");
        return 1;
    }

    printf("Message queue permissions changed to 600.\n");

    if (msgctl(msgid, IPC_STAT, &info) == -1)
    {
        perror("IPC_STAT");
        return 1;
    }

    printf("New permissions: %o\n",
           (unsigned int)(info.msg_perm.mode & 0777));

    return 0;
}
/*
 mkdir -p ~/Desktop/HandsOnList-2/ipcshared
cd ~/Desktop/HandsOnList-2/ipcshared

gcc ~/Desktop/HandsOnList-2/41/41.c -o 41
gcc ~/Desktop/HandsOnList-2/45/45.c -o 45

./41
./45
Message Queue Key: 1090651824
Message Queue ID: 1
Old permissions: 666
Message queue permissions changed to 600.
New permissions: 600
*/
