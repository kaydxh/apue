#include "../include/apue.h"

static int pfd1[2], pfd2[2];
//子进程使用pfd1，父进程使用pfd2，子进程等待父进程 read pdf1[0], 父进程通知子进程 write pd1[1] 子进程通知父进程 write pdf2[1]

void 
TELL_WAIT(void) {
    if (pipe(pfd1) < 0 || pipe(pfd2) < 0) {
        err_sys("pipe error");
    }
}

void
WAIT_CHILD(void) {
    char c;

    if (read(pfd2[0], &c, 1) != 1) {
        err_sys("read error");
    } 

    if (c != 'c') {
        err_quit("WAIT_CHILD: incorrect data");
    }
}

void
TELL_PARENT(pid_t pid) {
    if (write(pfd2[1], "c", 1) != 1) {
        err_sys("write error");
    }
}

void
WAIT_PARENT(void) {
    char c;
    if (read(pfd1[0], &c, 1) != 1) {
        err_sys("read error");
    }

    if (c != 'p') {
        err_quit("WAIT_PARENT: incorrect data");
    }
}

void 
TELL_CHILD(pid_t pid) {
    if (write(pfd1[1], "p", 1) != 1) {
        err_sys("write error");
    }
}


