#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

int sign;
int pid;

int i = 31;
int res = 0;

void my_handler(int nsig) {
  if (i == 31)
    sign = nsig == 12;
  else {
    res = res << 1;
    if ((nsig == 12) ^ sign) {
      res++;
    }
  }
  i--;
  kill(pid, SIGUSR1);
}

int main() {
  printf("My pid: %d \n", getpid());
  printf("Enter pid: ");
  scanf("%d", &pid);

  (void) signal(SIGUSR1, my_handler);
  (void) signal(SIGUSR2, my_handler);
  kill(pid, SIGUSR1);

  while(i >= 0);

  if (sign) {
    res += 1;
    res *= -1;
  }

  printf("Result: %d \n", res);
  return 0;
}
