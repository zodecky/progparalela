#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 15
#define SHM_KEY 0x2345
#define N_MSG 8

typedef struct msgbuf {
    long    mtype;
    char    mtext[MAX_SIZE];
} message_container;

int main() {
    int msqid;
    struct msqid_ds msgstat;
    message_container rbuf;

    if ((msqid = msgget(SHM_KEY, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    if (msgctl(msqid, IPC_STAT, &msgstat) < 0) {
        perror("msgctl");
        exit(1);
    }

    msgstat.msg_qbytes = sizeof(message_container);

    for(int i = 1; i <= 128; i++) {
        if (msgrcv(msqid, &rbuf, sizeof(message_container), 1, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }
        printf("msg_qnum: %ld\n", msgstat.msg_qnum);
        printf("%s\n", rbuf.mtext);
    }

    exit(0);


    return 0;
}