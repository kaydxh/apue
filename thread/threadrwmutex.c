#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <pthread.h>

struct foo {
    int f_count;
    pthread_rwlock_t f_rwlock;
};

struct foo * foo_alloc(void);
void foo_add(struct foo *fp);
void foo_release(struct foo *fp);

void *thread_func1(void *arg);
void *thread_func2(void *arg);
void *thread_func3(void *arg);

int
main() {
    pthread_t pid1, pid2, pid3;
    int err;
    void *pret;
    struct foo *fobj;
    fobj = foo_alloc();
    
    err = pthread_create(&pid1, NULL, thread_func1, (void *)fobj);
    if (err != 0) {
        perror("pthread_create error");
        exit(-1);
    }

    err = pthread_create(&pid2, NULL, thread_func2, (void *)fobj);
    if (err != 0) {
        perror("pthread_create error");
        exit(-1);
    }

    err = pthread_create(&pid3, NULL, thread_func3, (void *)fobj);
    if (err != 0) {
        perror("pthread_create error");
        exit(-1);
    }

    pthread_join(pid1, &pret);
    printf("thread 1 exit code is: %d\n", (int)pret);
    pthread_join(pid2, &pret);
    printf("thread 2 exit code is: %d\n", (int)pret);

    pthread_join(pid3, &pret);
    printf("thread 3 exit code is: %d\n", (int)pret);

    exit(0);
}


struct foo * 
foo_alloc(void) {
    struct foo *fobj;
    fobj = (struct foo *)malloc(sizeof(struct foo));
    if (NULL != fobj) {
        fobj->f_count = 0;

        if (pthread_rwlock_init(&fobj->f_rwlock, NULL) != 0) {
            free(fobj);
            return NULL;
        }
    }

    return fobj;
}

void
foo_add(struct foo *fp) {
    pthread_rwlock_wrlock(&fp->f_rwlock);
    fp->f_count++;
    printf("add f_count = %d\n", fp->f_count);
    pthread_rwlock_unlock(&fp->f_rwlock);
}

void
foo_release(struct foo *fp) {
    pthread_rwlock_wrlock(&fp->f_rwlock);
    fp->f_count--;
    printf("release f_count = %d\n", fp->f_count);
    if (fp->f_count == 0) {
        pthread_rwlock_unlock(&fp->f_rwlock);
        pthread_rwlock_destroy(&fp->f_rwlock);
        free(fp);
    } else {
        pthread_rwlock_unlock(&fp->f_rwlock);
    }
}

int
foo_search(struct foo *fp) {
    int count;
    pthread_rwlock_rdlock(&fp->f_rwlock);
    count = fp->f_count;
    printf("search f_count = %d\n", fp->f_count);
    pthread_rwlock_unlock(&fp->f_rwlock);

    return count;
}

void *
thread_func1(void *arg) {
    struct foo *fp = (struct foo *)arg;
    printf("thread 1 start.\n");
    foo_search(fp);
    printf("thread 1 exit.\n");
    pthread_exit((void *)1);
}

void *
thread_func2(void *arg) {
    struct foo *fp = (struct foo *)arg;
    printf("thread 2 start.\n");
    foo_add(fp);
    printf("thread 2 exit.\n");
    pthread_exit((void *)2);
}

void *
thread_func3(void *arg) {
    struct foo *fp = (struct foo *)arg;
    printf("thread 3 start.\n");
    foo_add(fp);
    foo_search(fp);
    printf("thread 3 exit.\n");
    pthread_exit((void *)3);
}