#include "../include/apue.h"
#include <setjmp.h>

#define TOK_ADD 5

jmp_buf jmpbuffer;

void do_line(char *);
void cmd_add(void);
int get_token(void);

char * tok_prt;

void
do_line(char *ptr) {
    int cmd;

    tok_prt = ptr;
    while ((cmd = get_token()) > 0) {
        switch (cmd) {
            case TOK_ADD:
                cmd_add();
                break;
        }
    }

}

void
cmd_add(void) {
    int token;

    token = -1; //get_token();
    if (token < 0) {
        longjmp(jmpbuffer, 1);
    }
}

int
get_token(void) {
    return TOK_ADD;
}


int
main(void) {
    char line[MAXLINE];

    int ret;
    if ((ret = setjmp(jmpbuffer)) != 0) {
        printf("error ret: %d\n", ret);
    }

    while (fgets(line, MAXLINE, stdin) != NULL) {
        do_line(line);
    }

    exit(0);
}