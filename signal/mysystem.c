#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

int
mysystem(const char *cmdstring) {
	pid_t pid;
	int   status;
	struct sigaction ignore, saveintr, savequit;
	sigset_t chldmask, savemask;

	//空命令退出
	if (cmdstring == NULL) {
		return (1);
	}

	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	//忽略SIGINT信号
	if (sigaction(SIGINT, &ignore, &saveintr) < 0) {
		return (-1);
	}
	//忽略SIGQUIT信号
	if (sigaction(SIGQUIT, &ignore, &savequit) < 0) {
		return (-1);
	}

	sigemptyset(&chldmask);
	sigaddset(&chldmask, SIGCHLD);
	//阻塞SIGCHLD信号
	if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0) {
		return (-1);
	}

	if ((pid = fork()) < 0) {
		status = -1;
	} else if (pid == 0) { //子进程
		//恢复SIGINT、SIGQUIT信号之前的处理方式
		sigaction(SIGINT, &saveintr, NULL);
		sigaction(SIGQUIT, &savequit, NULL);
		sigprocmask(SIG_SETMASK, &savemask, NULL);

		execl("/bin/sh", "sh", "-c", cmdstring, (char*) 0);
		_exit(127);
	} else {
		while (waitpid(pid, &status, 0) < 0) {
			if (errno != EINTR) { //除了EINTR错误
				status = -1;
				break;
			}
		}
	}

	if (sigaction(SIGINT, &saveintr, NULL) < 0) {
		return (-1);
	}

	if (sigaction(SIGQUIT, &savequit, NULL) < 0) {
		return (-1);
	}

	if (sigprocmask(SIG_SETMASK, &savemask, NULL) < 0) {
		return (-1);
	}

	return (status);
}

int
main() {
	printf("Print date:\n");
	mysystem("date");
	printf("Print process:\n");
	mysystem("ps");
	exit(0);
}

/*
[xhding@centos-linux signal]$ ./mysystem
Print date:
Thu Sep 14 12:46:55 CST 2017
Print process:
  PID TTY          TIME CMD
 3490 pts/0    00:00:00 bash
 7334 pts/0    00:00:00 mysystem
 7336 pts/0    00:00:00 ps

   */
