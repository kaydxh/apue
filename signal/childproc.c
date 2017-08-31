#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>

void sig_child(int signo) {
	int status;
	if (waitpid(-1, &status, 0) != -1) {
		//printf("child process exit. status = %d\n", status);
    	if (WIFEXITED(status)) {
       		 printf("normal termination, exit status = %d\n", WEXITSTATUS(status)); //正常退出，获取退出码
    	} 
	} else {
		perror("waitpid error");
		exit(-1);
	}
}

int
main() {
	pid_t pid;
	signal(SIGCHLD, sig_child);
	if ((pid = fork()) == -1) {
		perror("fork error");
	} else if (pid == 0) {
		printf("I am child process.\n");
		exit(1);
	} else {
		sleep(3);
		printf("I am parent process.\n");
	}

	exit(0);
}
