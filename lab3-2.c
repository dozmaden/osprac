#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid = fork();

  if (pid == -1){
    printf("ERROR! ");
  } else if (pid == 0) {
    printf("Is a child! ");
  } else {
    printf("Is a parent! ");
  }

  printf("Current pid = %d, cuttrentppid = %d \n", (int)getpid(), (int)getppid());

  return 0;
}
