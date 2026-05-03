#include "kernel/types.h"
#include "user.h"

/* function declarations */
void t_getpid();
void t_check_proc(const char *pid_str);
void t_check_proc_6();
void t_msgset(const char *msg_str);
void t_msgget();
void t_get_pgdir();
void t_validpg_num();


int
main(int argc, char *argv[]) {
    if(argc == 2) {
        if(strcmp(argv[1], "getpid") == 0) {
            t_getpid();
        } else
        if(strcmp(argv[1], "check_proc_6") == 0) {
            t_check_proc_6();
        } else
        if(strcmp(argv[1], "msgget") == 0) {
            t_msgget();
        } else
        if(strcmp(argv[1], "get_pgdir") == 0) {
            t_get_pgdir();
        } else
        if(strcmp(argv[1], "validpg_num") == 0) {
            t_validpg_num();
        }
        else  {
            goto usage;
        }
    } else 
    if(argc == 3) {
        if(strcmp(argv[1], "check_proc") == 0) {
            t_check_proc(argv[2]);
        } else 
        if(strcmp(argv[1], "msgset") == 0) {
            t_msgset(argv[2]);
        }
        else {
            goto usage;
        }
    } 
    else {
        goto usage;
    }

    exit(0);
usage:
    fprintf(2, "Usage: mytest getpid\n");
    fprintf(2, "Usage: mytest msgget\n");
    fprintf(2, "Usage: mytest get_pgdir\n");
    fprintf(2, "Usage: mytest validpg_num\n");
    fprintf(2, "Usage: mytest check_proc_6\n");
    fprintf(2, "Usage: mytest check_proc <pid>\n");
    fprintf(2, "Usage: mytest msgset <message>\n");
    exit(0);
}

/* function definitions */

void t_getpid() {
    int pid = my_getpid();
    fprintf(2, "pid: %d\n", pid);
}

void t_check_proc(const char *pid_str) {
    int pid = atoi(pid_str);
    fprintf(2, "pid %d is %s\n", pid, 
        check_proc(pid) == 0 ? "valid" : "invalid");
}

void t_check_proc_6() {
    for(int i = 1; i <= 6; i++) {
        fprintf(2, "pid %d is %s\n", i, 
            check_proc(i) == 0 ? "valid" : "invalid");
    }
}

void t_msgset(const char *msg_str) {
    /* read immediate from stack */
    if(set_msg(msg_str, strlen(msg_str)) < 0) {
        fprintf(2, "failed to set message\n");
    }
}

void t_msgget() {
    char buf[256];
    if(get_msg(buf, 256) < 0) {
        fprintf(2, "failed to get message\n");
    } else {
        fprintf(2, "%s\n", buf);
    }
}

void t_get_pgdir() {
    uint64 pgdir = get_pgdir();
    fprintf(2, "page directory: 0x%lx\n", pgdir);
}

void t_validpg_num() {
    int num = validpg_num();
    fprintf(2, "valid page count: %d\n", num);
}
