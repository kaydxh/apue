#include "../include/apue.h"
#include <pwd.h>

static void
my_alarm(int signo) {
    struct passwd *rootptr;

    printf("in signal handler\n");
    if ((rootptr = getpwnam("root")) == NULL) {
        err_sys("getpwnam(root) error");
    } else {
		printf("getpwnam(root) succ\n");
	}

    unsigned int sec = alarm(1);
	printf("sec = %d\n", sec);
	
}

int
main(void) {
    struct passwd *ptr;
    signal(SIGALRM, my_alarm);
	alarm(1);

    for (;;) {
#if 1
        if ((ptr = getpwnam("xhding")) == NULL) {
            err_sys("getpwnam error");
        }
        if (strcmp(ptr->pw_name, "xhding") != 0) {
            printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
        } else {
//            printf("return value not  corrupted!, pw_name = %s\n", ptr->pw_name);

		}

#endif
	//	sleep(1);
    }
}
