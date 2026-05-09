#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int global = 0;

int main(void) {
    printf("init: global = %d\n", global);
    int pid = clone();

    if (pid == 0) {
        global = 1;
        printf("Child Thread: global = %d\n", global);
        exit(0);
    } else {
        wait(0);
        printf("Parent Thread: global = %d\n", global);
    }
    exit(0);
}