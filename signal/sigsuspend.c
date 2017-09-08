#include "../include/apue.h"

static void sig_int(int);

int
main(void) {
	sigset_t newmask, oldmask, waitmask;

	pr_mask("program start: ");

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		err_sys("signal(SIGINT) error");
	}

	sigemptyset(&waitmask);
	sigaddset(&waitmask, SIGUSR1);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);

	//SIG_BLOCK, 进程新的信号屏蔽字是当前信号屏蔽字和newmask指向的信号集的并集
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		err_sys("SIG_BLOCK error");
	}

	pr_mask("in critical region: ");

	//修改进程屏蔽字，在捕捉信号之前，将进程挂起
	if (sigsuspend(&waitmask) != -1) {
		err_sys("sigsuspend error");
	}

	pr_mask("after return from sigsuspend: ");

	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		err_sys("SIG_SETMASK error");
	}

	pr_mask("program exit: ");
	
	exit(0);
}

static void
sig_int(int signo) {
	pr_mask("\nin sig_int: ");
}

/*output
[xhding@centos-linux signal]$ ./sigsuspend
program start:
in critical region:  SIGINT
^C
in sig_int:  SIGINT SIGUSR1
after return from sigsuspend:  SIGINT
program exit:
*/
