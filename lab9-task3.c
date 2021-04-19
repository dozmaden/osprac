#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <errno.h>

void try_semop(int sem_id, int val) {
  struct sembuf mybuf;
  mybuf.sem_num = 0;
  mybuf.sem_op  = val;
  mybuf.sem_flg = 0;
  if (semop(sem_id, &mybuf, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }
}
// увеличение семафора
void A(int sem_id, int val) {
  try_semop(sem_id, val);
}
// уменьшение семафора
void D(int sem_id, int val) {
  try_semop(sem_id, -val);
}
// сравнение на 0
void Z(int sem_id) {
  try_semop(sem_id, 0);
}

// Родитель делает D(1), чтобы написать своё предложение. Реёбнок Z (проверка нулю), и увеличивает сем. на 2.
// Для того, чтобы в семафоре опять было 0, нужно чтобы родитель зашёл в цикл и вышел из него.

int main()
{
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

    if ((semid = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
        	printf("Can\'t create semaphore set\n");
        	exit(-1);
        } else if ((semid = semget(key, 1, 0)) < 0) {
        	printf("Can\'t find semaphore\n");
        	exit(-1);
        }
    } else {
        A(semid, 2);
    }
    // создал семафор

    int N;
    printf("Enter N: \n");
    scanf("%d", &N);
	
    while (N < 2) {
        printf("N must be greater or equal to 2 \n");
	printf("Enter N: \n");
	scanf("%d", &N);
    }
    
    result = fork();
	
    if (result < 0) {
        printf("Couldn't fork child! \n");
        exit(-1);
    }
    else if (result > 0) {
	// логика отца

        for (size_t i = 0; i < N; i++)
        {
            D(semid, 1);
		
            if (i != 0) {
                size = read(fd[0], resstring, 14);
                if (size < 0) {
			printf("Can\'t read string \n");
                	exit(-1);
                }
                printf("N = %d, parent read message: %s \n", i, resstring);
            }

            size = write(fd[1], "Hello, world!", 14);
            
            if (size != 14) {
                printf("Can\'t write all string to pipe\n");
                exit(-1);
            }
		
            D(semid, 1);
        }
        close(fd[0]);
    }
    else {
        // номер ребёнка
        int cnt = 0;
	
	for (int i = 0; i < N; ++i) {
            Z(semid);

            size = read(fd[0], resstring, 14);
            if (size < 0) {
                printf("Can\'t read string \n");
                exit(-1);
            }
		
            printf("N = %d, child read message: %s \n", ++cnt, resstring);

            size = write(fd[1], "Hello, parent", 14);
            if (size != 14) {
                printf("Can't write string %d \n", size);
                exit(-1);
            }

            A(semid, 2);
        }
        close(fd[1]);
        close(fd[0]);
    }
    return 0;
}
