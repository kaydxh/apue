#include "../include/apue.h"
#include <fcntl.h>

int
set_fl(int fd, int flags) {
	int val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
		err_sys("fcntl F_GETFL error");
	}

	val |= flags;

	if (fcntl(fd, F_SETFL, val) < 0) {
		err_sys("fcntl F_SETFL error");
	}

}

int main(int argc, char * argv[]) {

	if (argc != 2) {
		err_quit("usage: fileflags <descriptor#>");
	}

	set_fl(atoi(argv[1]), O_WRONLY);

	return 0;
}