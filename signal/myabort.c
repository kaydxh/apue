#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
myabort(void) {
	sigset_t mask;
	struct sigaction action;

	sigaction(SIGABRT, NULL, &action);
	//如果调用者没有忽略SIGABRT,就重置SIGABTT
	if (action.sa_handler == SIG_IGN) { //这里main函数调用没有忽略，所以不会进入该if
		action.sa_handler = SIG_DFL;
		sigaction(SIGABRT, &action, NULL);
	}

	if (action.sa_handler == SIG_DFL) {
		fflush(NULL); //冲洗所有标志I/O流（并不关闭它们）
	}

	//调用者不能阻塞SIGABRT,确定该信号不阻塞. 
	//sigfillset()用来将参数set信号集初始化，然后把所有的信号加入到此信号集里即将所有的信号标志位置为1，屏蔽所有的信号。它是一个宏实现，如下所示：
	//#define sigfillset(ptr) ( *(ptr) = ~(sigset_t)0, 0), 因为函数sigfillset必须返回0，所以使用C语言里面的逗号运算符，它将逗号运算符后的值做为表达式的值返回

	//除了SIGABRT，阻塞其他所有信号，这样如果调用kill返回了，则该进程一定已捕捉到该信号，并且也从该信号处理程序返回
	sigfillset(&mask);
	sigdelset(&mask, SIGABRT);
	sigprocmask(SIG_SETMASK, &mask, NULL);
	kill(getpid(), SIGABRT);

	fflush(NULL);
	action.sa_handler = SIG_DFL; //SIG_DFL：默认信号处理程序
	sigaction(SIGABRT, &action, NULL);//重置默认
	sigprocmask(SIG_SETMASK, &mask, NULL);
	kill(getpid(), SIGABRT);
	exit(1);//永远不会执行
}

static void
sig_abort(int signo) {
	printf("abort signal.\n");
}

int
main() {
	signal(SIGABRT, sig_abort);
	myabort();
	pause();
	exit(0);
}

//output
//[xhding@centos-linux signal]$ ./myabort
//abort signal.
//Aborted (core dumped)
