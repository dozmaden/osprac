#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
 
int main(void)
{
	int fd[2];
	char* test = "a";
	size_t size;
	int pipe_size = 0;

	if (pipe(fd) < 0) {
		printf("Can\'t open pipe\n");
		exit(-1);
  	}

	fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFD | O_NONBLOCK));

	do{
		size = write(fd[1], test, 1);
		if (size != 1){
			break;
		}
		pipe_size++;
	} while(size == 1);

    	printf("%d bytes in pipe \n\r", pipe_size * sysconf(_SC_PAGESIZE));
	// умножаем количество pages (16 в моём случае на их размер)
	// 16 * 4096
    	return 0;
}
