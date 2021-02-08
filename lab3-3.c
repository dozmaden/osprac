#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
	printf("All arguments: \n");
	for (int i = 0; i < argc; ++i) {
		printf("arg[%d] = %s \n", i, argv[i]);
	}

	printf("\n Parameters (envp's): \n");
	int cnt = 0;
	while (envp[cnt] != NULL) {
		printf("envp[%d] = %s \n", cnt, envp[cnt++]);
	}

	return 0;
}
