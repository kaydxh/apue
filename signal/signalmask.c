#include "../include/apue.h"

static void sig_quit(int);

int
main(void) {
	sigset_t newmask, oldmask, pendmask;

	if (signal(SIGQUIT, sig_quit) == SIG_ERR) {
		err_sys("can't catch SIGQUIT");
	}

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT); ////添加一个退出信号
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) { ////将newmask信号机设置为阻塞，原信号集保存在oldmask中
		err_sys("SIG_BLOCK error");
	}
	
	sleep(5);

	if (sigpending(&pendmask) < 0) { //获取阻塞的信号集
		err_sys("sigpending error");
	}

	if (sigismember(&pendmask, SIGQUIT)) { //判断SIGQUIT是否是阻塞的,SIGQUIT目前是阻塞的，因此没有到sig_quit函数中
		printf("\nSIGQUIT pending\n");
	}

	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) { //恢复原来的信号集
		err_sys("SIG_SETMASK error");
	}

	printf("SIGQUIT unblocked\n");

	sleep(5);
	exit(0);
}

static void
sig_quit(int signo) {
	printf("caught SIGQUIT\n");

	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR) {
		err_sys("can't reset SIGQUIT");
	}
}
