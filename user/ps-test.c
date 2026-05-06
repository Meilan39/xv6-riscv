#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user.h"

void print_pstat(struct pstat *ps) {
    printf("pid\ttickets\tstride\tpass\titerations\n");    
    for(int i = 0; i < NPROC; i++) {
        if(ps->inuse[i]) {
            printf("%d\t%d\t%d\t%d\t%d\n", 
                ps->pid[i], ps->ticket[i], ps->stride[i], ps->pass[i], ps->pass[i] / ps->stride[i]);
        } 
    }
}

int main(int argc, char *argv[]) {
    int pid = fork();
    if(pid < 0){
        printf("fork failed\n");
        exit(1);
    }

    if(pid == 0){
        settickets(100);
        for(volatile int i = 0; i < 50000000; i++); // don't optimize
        exit(0);
    } else {
        settickets(20);
        for(volatile int i = 0; i < 50000000; i++);
        
        struct pstat ps;
        if(getpinfo(&ps) < 0){
            fprintf(2, "ps-test: getpinfo failed\n");
        } else {
            print_pstat(&ps);
        }
        wait(0);
    }
    exit(0);
}
