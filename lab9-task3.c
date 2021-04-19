#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>

// decreasing
int P(int semid, struct sembuf* buffer) {
    buffer->sem_num = 0;
    buffer->sem_op = -1;
    buffer->sem_flg = 0;
    return semop(semid, buffer, 1);
}

// increasing
int V(int semid, struct sembuf* buffer) {
    buffer->sem_num = 0;
    buffer->sem_op = 1;
    buffer->sem_flg = 0;
    return semop(semid, buffer, 1);
}

int main()
{
    // семафор = 0
    // P - уменьшает на 1
    // V - увеличивает на 1

    struct sembuf mybuf;
    int semid;

    int fd[2], result;
    size_t size = 0;

    key_t key;
    char pathname[] = "lab9-task3.c";

    char resstring[14];

    if (pipe(fd) < 0) {
        printf("Can't create pipe of parent\n");
        exit(-1);
    }	

    // создали пайпы для родителей и ребёнка

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666)) < 0) {
        // создаю семафор
        printf("No semaphore found. Creating... \n");
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can't get semid \n");
            exit(-1);
        }
        printf("Successfully created semaphore! \n");
    }

    result = fork();

    if (result < 0) {
        printf("Couldn't fork child! \n");
        exit(-1);
    }
    else if (result > 0) {
	// логика отца

        int N;
        printf("Enter N: \n");
        scanf("%d", &N);

        while (N < 2) {
            printf("N must be greater or equal to 2 \n");
            printf("Enter N: \n");
            scanf("%d", &N);
        }

        for (size_t i = 0; i < N; i++)
        {
            size = write(fd[1], "Hello, world!", 14);
            // пишу ребёнку
            if (size != 14) {
                printf("Can\'t write all string \n");
                exit(-1);
            } else{
		printf("N = %d, parent wrote to child \n", i + 1);
	    }

	    // передаём эстафету ребёнку
            V(semid, &mybuf);
            P(semid, &mybuf);

            size = read(fd[0], resstring, 14);
	    // теперь родитель читает ребёнка 
            if (size != 14) {
                printf("Can\'t read from child \n");
                exit(-1);
            } else{
            	printf("N = %d, parent read from child = %s\n", i + 1, resstring);
	    }
        }
        close(fd[0]);
        close(fd[1]);
    }
    else {
        // номер ребёнка
        int cnt = 0;

        while(1) {
            P(semid, &mybuf);

            size = read(fd[0], resstring, 14);
	    printf("N = %d, child read: %s\n", ++cnt, resstring);
            // читаем родителя

            if (size < 0) {
		break;
            }
 	    
            // пишем родителю
            if (write(fd[1], "Hello, parent", 14) != 14) {
                printf("Can\'t write all string. \n");
                exit(-1);
            }
 
            // теперь передаём эстафету родителю
            V(semid, &mybuf); 
        }
        close(fd[1]);
        close(fd[0]);
        printf("End program.\n");
    }
    return 0;
}
