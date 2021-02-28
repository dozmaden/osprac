#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
    char *source;
    int *size_p; // size pointer
    int size = 0;

    int shmid;
    char pathname[] = "lab7-read.c"; // path to 2nd prg
    key_t key;

    FILE *file = fopen(pathname, "r");
    
    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    rewind(file);
    
    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    
    if ((shmid = shmget(key, sizeof(int) + sizeof(char) * size, 0666|IPC_CREAT)) < 0) {
        printf("Can\'t create shared memory\n");
        exit(-1);
    }

    if ((size_p = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    *size_p = size;
    source = (char*)(size_p + 1);
    
    for (int i = 0; i < size; i++){
        source[i] = fgetc(file);
    }
        
    fclose(file);
        
    if (shmdt(size_p) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }

    return 0;
}
