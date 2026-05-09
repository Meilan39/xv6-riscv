#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int count = 0;

int main(void) {
    uint sem = sem_alloc();
    sem_init(sem, 1);

    if (clone() == 0) {
        for(int i = 0; i < 10000; i++){
            sem_wait(sem);
            count++;
            sem_post(sem);
        }
        exit(0);
    } else {
        for(int i = 0; i < 10000; i++){
            sem_wait(sem);
            count++;
            sem_post(sem);
        }
        wait(0);
        printf("Sum : %d\n", count); // should be 20000
    }

    sem_free(sem);
    exit(0);
}
