#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

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

    msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid == -1)
    {
        perror("msgget");
        return 1;
    }

    if (msgctl(msgid, IPC_STAT, &info) == -1)
    {
        perror("msgctl");
        return 1;
    }

    printf("Message Queue ID: %d\n", msgid);

    printf("\n--- Access Information ---\n");
    printf("Permissions: %o\n",
           (unsigned int)(info.msg_perm.mode & 0777));
    printf("UID: %u\n", (unsigned int)info.msg_perm.uid);
    printf("GID: %u\n", (unsigned int)info.msg_perm.gid);

    printf("\n--- Time Information ---\n");
    printf("Last message sent: %s", ctime(&info.msg_stime));
    printf("Last message received: %s", ctime(&info.msg_rtime));
    printf("Last queue change: %s", ctime(&info.msg_ctime));

    printf("\n--- Queue Information ---\n");
    printf("Current queue size: %lu bytes\n",
           (unsigned long)info.msg_cbytes);
    printf("Number of messages: %lu\n",
           (unsigned long)info.msg_qnum);
    printf("Maximum bytes allowed: %lu\n",
           (unsigned long)info.msg_qbytes);

    printf("\n--- Process Information ---\n");
    printf("PID of last msgsnd: %d\n", (int)info.msg_lspid);
    printf("PID of last msgrcv: %d\n", (int)info.msg_lrpid);

    return 0;
}
