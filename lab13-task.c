#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

int main(){
    // directory for all links
    char* dir = "files";
    printf("Program will look at directory: %s \n", dir);
   
    DIR *dirloc = opendir(dir);
    if(dirloc == NULL){
        printf("Directory not found! \n");
        exit(1);
    }
    
    // creating the starting file
    char* origin = malloc(sizeof(16));
    sprintf(origin, "files/%d", 0);
    FILE* f = fopen(origin, "w");
    fclose(f);

    int cnt = 1;
    while(1){
    	char* prev = malloc(sizeof(16));
	sprintf(prev, "%d", cnt - 1);

        char* next = malloc(sizeof(16));
	sprintf(next, "files/%d", cnt);

        if(symlink(prev, next) == -1){
            exit(-1);
        }

        if(fopen(next, "r") == NULL){
            printf("Max recursion depth: %d \n", cnt - 1);
            // decrement cnt since we don't count last file
            exit(1);
        }

        cnt++;
        free(prev);
        free(next);
    }

    if (closedir(dirloc) != 0) {
        printf("Can't close dir! \n");
        return(-1);
    }

    return 0;
}
