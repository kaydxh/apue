#include "../include/apue.h"
#include <pthread.h>

pthread_t ntid;

void
printids(const char *s) {
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, 
        (unsigned long)tid, (unsigned long)tid);
}

void *
thr_fn(void *arg) {
    printids("new thread: ");
    return ((void *)0);
}

int
main(void) {
    int err;
    err = pthread_create(&ntid, NULL, thr_fn, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread");
    }

    printids("main thread: ");
    sleep(1);
    exit(0);
}

/*
[xhding@centos-linux thread]$ ./createthread
main thread:  pid 21827 tid 140398973277952 (0x7fb12eedd700)
new thread:  pid 21827 tid 140398973269760 (0x7fb12eedb700)
*/