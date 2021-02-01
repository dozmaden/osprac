#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid, ppid;

  pid  = getpid();
  ppid = getppid();

  printf("Current pid = %d, cuttrentppid = %d \n", (int)pid, (int)ppid);
  // The pid id increases everytime a new process is created
  return 0;
}
