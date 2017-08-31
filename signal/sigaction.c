#include "../include/apue.h"

typedef void Sigfunc(int);

Sigfunc *
my_signal(int signo, Sigfunc *func) {
	struct sigaction act, oact;
	act.sa_handler = func; //设置中断处理程序
	sigemptyset(&act.sa_mask); //初始化信号集
	act.sa_flags = 0;
	if (signo == SIGALRM) { //将SIGALRM信号设置为系统调用不会自动重启
#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;
#endif
	} else { //其余信号设置为系统调用自动重启
		act.sa_flags |= SA_RESTART;
	}

	if (sigaction(signo, &act, &oact) < 0) {
		return (SIG_ERR);
	}

	return (oact.sa_handler);
}

static void sig_func(int signo) {
	printf("received a SIGALRM signal.\n");
}

int
main() {
	printf("starting.\n");
	my_signal(SIGALRM, sig_func);
	alarm(2);
	pause();
	exit(0);
}
