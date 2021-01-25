#include "stdio.h"
#include "unistd.h"

int main(){
	int usid = getuid();
	int group = getgid();
	prinf("User id is : %d , Group id: %d", usid, group);
}
