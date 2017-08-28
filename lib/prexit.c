#include "../include/apue.h"
#include <sys/wait.h>

void
pr_exit(int status) {
    if (WIFEXITED(status)) {
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status)); //正常退出，获取退出码
    } else if (WIFSIGNALED(status)) {
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status), //异常退出，获取退出码
    

#ifdef WCOREDUMP
    WCOREDUMP(status) ? " (core file generated)" : ""); //某些实现用 WCOREDUMP检查是否产生core
#else
    "");
#endif

    } else if (WIFSTOPPED(status)) { //若为当前暂停子进程的返回状态，则为真
        printf("child stopped, signal number = %d\n", WSTOPSIG(status) );
    }
}