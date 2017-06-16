#include "../include/apue.h"

#define BUFFSIZE 4096

int
main(void) {
	int n;
	char buf[BUFFSIZE];

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) { //STDIN_FILENO,STDOUT_FILENO 定义在unistd.h
		if (write(STDOUT_FILENO, buf, n) != n) { //写的数据不是n就是失败
			err_sys("write error");
		}
	}

	if (n < 0) {
		err_sys("read error");
	}

	exit(0);
}