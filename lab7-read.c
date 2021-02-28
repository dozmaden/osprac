#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
    char *source;
    int *size_p;
    int size = 0;

    int *array;
    int shmid;
    char pathname[] = "lab7-read.c";
    key_t key;

    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    
    if ((shmid = shmget(key, sizeof(int) + sizeof(char) * size, 0)) < 0) {
        printf("Can\'t create shared memory\n");
        exit(-1);
    }

    if ((size_p = (int*)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    size = *size_p;
    source = (char*)(size_p + 1);
    
    for (int i = 0; i < size; i++){
        putchar(source[i]);
    }
    
    if (shmdt(size_p) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        printf("Can't delete shared memory\n");
        exit(-1);
    }

    return 0;
}
