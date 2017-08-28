#include "../include/apue.h"
#include <sys/wait.h>

int
main(void) {
    pid_t pid;
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid > 0) {
            exit(2);
        }

        sleep(2);
        printf("second child, parent pid = %ld\n", (long)getppid() );
        exit(1);
    }

    int status;

    if (waitpid(pid, &status, 0) != pid) { //等待先退出的子进程（为第一个子进程），这里退出码为2
        err_sys("waitpid error");
    }

    pr_exit(status); 

    printf("%d\n", getpid());

    exit(0);
}

//$ ./wait2 //运行程序，
//$ second child, parent pid = 1