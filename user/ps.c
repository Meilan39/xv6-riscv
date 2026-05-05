#include "kernel/pstat.h"
#include "kernel/types.h"
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
    struct pstat ps;
    if(getpinfo(&ps) < 0){
        fprintf(2, "ps: getpinfo failed\n");
        exit(1);
    }
    print_pstat(&ps);
    exit(0);
}