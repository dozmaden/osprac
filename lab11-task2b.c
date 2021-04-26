#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int msqid;
    char pathname[] = "lab11-task2a.c";

    key_t key;
    int len;

    struct mymsgbuf {
        long mtype;
        struct {
            short sinfo;
        } info;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }

    printf("Progam B starts receiving messages from program A.\n");

    for (int i = 0; i < 5; ++i) {
        int infolen = sizeof(mybuf.info);
        if (len = msgrcv(msqid, (struct msgbuf *) &mybuf, infolen, 1, 0) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }

        printf("Program B received: message type = %ld, sInfo = %i\n", mybuf.mtype, mybuf.info.sinfo);
    }

    printf("Program B received all messages from program A.\n");

    printf("Program B will now send messages to program A.\n");

    for (int i = 0; i < 5; ++i) {
        mybuf.mtype = 2;
        mybuf.info.sinfo = 360;

        if (msgsnd(msqid, &mybuf, sizeof(mybuf.info), 0) < 0) {
            printf("Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }

    printf("Program B sent all messages to program A.\n");

    return 0;
}




