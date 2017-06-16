#include "../include/apue.h"
#include <sys/wait.h>

int 
main(void) {
	char buf[MAXLINE];
	pid_t pid;
	int status;

	printf("%% ");
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == '\n') {
			buf[strlen(buf) - 1] = 0;
		}

		if ((pid = fork()) < 0) {
			err_sys("fork error");
		} else if (pid == 0) { //child
			execlp(buf, buf, (char*)0); // execlp从path路径中查找第一个参数的可执行文件名称，第二个参数开始作为，
					//argv[0], argv[1]... 最后一个参数必须为0
			err_ret("could't execute: %s", buf);
			exit(127);
		} 

		//parent
		if ((pid = waitpid(pid, &status, 0)) < 0) {
			err_sys("waitpid error");
		}

		printf("%% ");
	}

	exit(0);
}