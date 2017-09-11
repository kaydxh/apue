#include "../include/apue.h"
#include <stdio.h>

static volatile sig_atomic_t sigflag;
static sigset_t newmask, oldmask, zeromask;

static void
sig_usr(int signo) {
	printf("signo = %d\n", signo);
	sigflag = 1;
}

void
TELL_WAIT(void) {
	if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
		err_sys("signal(SIGUSR1) error");
	}

	if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
		err_sys("signal(SIGUSR2) error");
	}

	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
	sigaddset(&newmask, SIGUSR2);

	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		err_sys("SIG_BLOCK error");
	}
}

void
TELL_PARENT(pid_t pid) {
	kill(pid, SIGUSR2);
}

void
WAIT_PARENT(void) {
	while (sigflag == 0) {
		sigsuspend(&zeromask);
	}

	sigflag = 0;

	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		err_sys("SIG_SETMASK error");
	}
}

void
TELL_CHILD(pid_t pid) {
	kill(pid, SIGUSR1);
}

void
WAIT_CHILD(void) {
	while (sigflag == 0) {
		sigsuspend(&zeromask);
		
	}

	sigflag = 0;

	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		err_sys("SIG_SETMASK error");
	}
}

static void
printcharacter(const char * str) {
	const char *ptr;
	setbuf(stdout, NULL);
	for (ptr = str; *ptr != '\0'; ptr++) {
		putc(*ptr, stdout);
	}
}

int
main() {
	pid_t pid;
	TELL_WAIT();
	pid = fork();

	switch(pid) {
		case -1:
			perror("fork() error");
			exit(-1);

		case 0:
		//	WAIT_PARENT();
			printcharacter("output from child process.\n");
			TELL_PARENT(getppid());
			break;

		default:
			WAIT_CHILD();
			printcharacter("output from parent process.\n");
			//TELL_CHILD(pid);
	}

	sleep(1);
	exit(0);
}
