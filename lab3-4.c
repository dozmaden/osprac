#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
	pid_t pid = fork();

	if (pid == -1) {
		printf("ERROR! \n");
	} else if (pid == 0) {
		printf("This is a child! \n");
		printf("The child will now attempt to start another program! \n");
		execle("/bin/cat", "/bin/cat", "lab3-3.c", 0, envp);

		printf("Error happened when starting a program! \n");
		exit(-1);
	} else {
		printf("This is a parent. ID: %d\n", (int)getpid());
		printf("Parent's child: %d\n", (int)pid);
	}

	return 0;
}

