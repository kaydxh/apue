#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <pthread.h>

struct foo {
	int f_count;
	pthread_mutex_t f_lock;
};

struct foo * foo_alloc(void);
void foo_add(struct foo *fp);
void foo_release(struct foo *fp);

void *thread_func1(void *arg);
void *thread_func2(void *arg);

int
main() {
	pthread_t pid1, pid2;
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

	pthread_join(pid1, &pret);
	printf("thread 1 exit code is: %d\n", (int)pret);
	pthread_join(pid2, &pret);
	printf("thread 2 exit code is: %d\n", (int)pret);

	exit(0);
}


struct foo * 
foo_alloc(void) {
	struct foo *fobj;
	fobj = (struct foo *)malloc(sizeof(struct foo));
	if (NULL != fobj) {
		fobj->f_count = 0;

		if (pthread_mutex_init(&fobj->f_lock, NULL) != 0) {
			free(fobj);
			return NULL;
		}
	}

	return fobj;
}

void
foo_add(struct foo *fp) {
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	printf("f_count = %d\n", fp->f_count);
	pthread_mutex_unlock(&fp->f_lock);
}

void
foo_release(struct foo *fp) {
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count--;
	printf("f_count = %d\n", fp->f_count);
	if (fp->f_count == 0) {
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
		pthread_mutex_unlock(&fp->f_lock);
	}
}

void *
thread_func1(void *arg) {
	struct foo *fp = (struct foo *)arg;
	printf("thread 1 start.\n");
	foo_release(fp);
	printf("thread 1 exit.\n");
	pthread_exit((void *)1);
}

void *
thread_func2(void *arg) {
	struct foo *fp = (struct foo *)arg;
	printf("thread 2 start.\n");
	foo_add(fp);
	foo_add(fp);
	printf("thread 2 exit.\n");
	pthread_exit((void *)2);
}

/*
[xhding@centos-linux thread]$ ./threadmutex
thread 2 start.
f_count = 1
f_count = 2
thread 2 exit.
thread 1 start.
f_count = 1
thread 1 exit.
thread 1 exit code is: 1
thread 2 exit code is: 2
*/



