#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 15
#define SHM_KEY 0x2345
#define N_MSG 1

typedef struct msgbuf
{
    long mtype;
    char mtext[MAX_SIZE];
} message_container;

int main()
{
    int msqid;
    struct msqid_ds msgstat;
    message_container sbuf;

    if ((msqid = msgget(SHM_KEY, 0666 | IPC_CREAT)) < 0)
    {
        perror("msgget");
        exit(1);
    }

    if (msgctl(msqid, IPC_STAT, &msgstat) < 0) // pega
    {
        perror("msgctl");
        exit(1);
    }


    sbuf.mtype = 1;
    msgstat.msg_qbytes = sizeof(message_container) * N_MSG; // muda

    if (msgctl(msqid, IPC_SET, &msgstat) < 0) // commit
    {
        perror("msgctl");
        exit(1);
    }

    for (int i = 1; i <= 128; i++)
    {
        sprintf(sbuf.mtext, "mensagem %d", i);
        if (msgsnd(msqid, &sbuf, sizeof(message_container), 0) < 0)
        {
            perror("msgsnd");
            exit(1);
        }
    }

    return 0;
}