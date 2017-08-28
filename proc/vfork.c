 
#include "../include/apue.h"

int globvar = 6;
char buf[] = "a write to stdout\n";

int
main(void) {
    int var;
    pid_t pid;

    var = 88;
    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1) { //a write to stdout
        err_sys("write error");
    }

    printf("before vfork\n");

    if ((pid = vfork()) < 0) {
        err_sys("fork error");
    } else if(pid == 0) { //child
        globvar++;
        var++;
        exit(0);
    } else { //parent
        sleep(2);
    }

    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var); //父进程数据也变化
    //pid = 4907, glob = 7, var = 89
    exit(0);
}


