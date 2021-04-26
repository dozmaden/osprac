#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main() {
	int msqid; // ipc дескриптор
	char pathname[] = "lab11-task1a.c";

	key_t key; //ключ
	int len; // длина информативной части сообщения

	// структура из семинара
	struct mymsgbuf {
		long mtype;
		struct {
			short sinfo;
			float finfo;
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

	while (1) {
		// получаем длину всей структуры, описывающей информатив. часть
		if (len = msgrcv(msqid, &mybuf, sizeof(mybuf.info), 0, 0) < 0) {
			printf("Can't receive message from queue\n");
			exit(-1);
		}

		if (mybuf.mtype == LAST_MESSAGE) {
			msgctl(msqid, IPC_RMID, NULL);
			exit(0);
		}

		printf("message type = %ld. sInfo = %i. fInfo = %f.\n", mybuf.mtype, mybuf.info.sinfo, mybuf.info.finfo);
	}

	return 0;
}




