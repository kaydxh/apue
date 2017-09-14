#include "../include/apue.h"

static void
sig_alrm(int signo) {
	printf("sig_alrm in\n");
}

unsigned int
mysleep(unsigned int seconds) {
	struct sigaction newact, oldact;
	sigset_t newmask, oldmask, suspmask;
	unsigned int unslept;

	//设置信号处理函数
	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	sigaction(SIGALRM, &newact, &oldmask);

	//阻塞SIGALRM和保存目前的signal mask
	sigemptyset(&newmask);
	sigaddset(&newact, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);

	//设置SIGALRM经过senconds后发送给目前的进程
	alarm(seconds);
	suspmask = oldmask;

	//确保SIGALRM不是阻塞的
	sigdelset(&suspmask, SIGALRM);
	
	//捕获信号
	sigsuspend(&suspmask);

	//已捕获到信号，SIGALRM目前是阻塞的
	//取消之前的闹钟，返回剩下的时间
	unslept = alarm(0);

	sigaction(SIGALRM, &oldact, NULL);

	sigprocmask(SIG_SETMASK, &oldmask, NULL);

	return (unslept);
}


int
main() {
	int i;
	printf("Program starting.\n");
	printf("sleep 5 seconds...\n");

	for (i = 1; i < 5; ++i) {
		printf("The %dth second\n", i);
		mysleep(1);
	}

	printf("wake up.\n");
	exit(0);
}

