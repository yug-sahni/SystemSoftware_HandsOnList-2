#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct message
{
    long type;
    char text[100];
};

int main()
{
    key_t key;
    int msgid;
    struct message msg;

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

    printf("Enter message: ");
    if (fgets(msg.text, sizeof(msg.text), stdin) == NULL)
        return 1;

    msg.type = 1;

    if (msgsnd(msgid, &msg, strlen(msg.text) + 1, 0) == -1)
    {
        perror("msgsnd");
        return 1;
    }

    printf("Message sent successfully.\n");
    printf("Message Queue ID: %d\n", msgid);

    return 0;
}
/*
 yugsahni@Ubuntu:~/Desktop/HandsOnList-2/43$ gcc 43.c -o 43
yugsahni@Ubuntu:~/Desktop/HandsOnList-2/43$ ./43
Enter message: hellllnaaa
Message sent successfully.
Message Queue ID: 2
yugsahni@Ubuntu:~/Desktop/HandsOnList-2/43$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x4102069b 0          yugsahni   666        0            0           
0x4102069d 1          yugsahni   666        0            0           
0x410206a1 2          yugsahni   666        12           1           

*/
