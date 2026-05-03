#include "kernel/types.h"
#include "user.h"

/* function declarations */
void t_getpid();
void t_check_proc(const char *pid_str);
void t_check_proc_6();

int
main(int argc, char *argv[]) {
    if(argc == 2) {
        if(strcmp(argv[1], "getpid") == 0) {
            t_getpid();
        } else
        if(strcmp(argv[1], "check_proc_6") == 0) {
            t_check_proc_6();
        } 
        else  {
            goto usage;
        }
    } else if(argc == 3) {
        if(strcmp(argv[1], "check_proc") == 0) {
            t_check_proc(argv[2]);
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
    fprintf(2, "Usage: mytest check_proc pid\n");
    fprintf(2, "Usage: mytest check_proc_6\n");
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