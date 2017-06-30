#ifndef _APUE_H
#define _APUE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MAXLINE 4096


void err_quit(const char *, ...) __attribute__((noreturn)); //没有返回值，调用了exit(),因为abort()和exit()等一些库函数没有返回值
											//控制不会再返回caller
void err_sys(const char *, ...)  __attribute__((noreturn)); //没有返回值，调用了exit(),因为abort()和exit()等一些库函数没有返回值
											//控制不会再返回caller

void err_dump(const char *fmt, ...) __attribute__((noreturn));

void err_ret(const char *fmt, ...);



#endif