#include "kernel/types.h"
#include "user/user.h"

#define rep 5

int main(void) {
    int p1[2], p2[2];
    int start, end;
    char msg[] = "a";
    int n = 1;

    pipe(p1);
    pipe(p2);

    if(fork() == 0) {
        close(p1[1]);
        close(p2[0]);
        for(int i = 0; i < rep; i++) {
            if(read(p1[0], msg, n) < n) {
                fprintf(2, "read error\n");
                exit(1);
            }
            if(write(p2[1], msg, n) < n) {
                fprintf(2, "write error\n");
                exit(1);
            }
        }
        close(p1[0]);
        close(p2[1]);
        exit(0); // exit
    } else {
        close(p1[0]);
        close(p2[1]);

        start = uptime();
        for(int i = 0; i < rep; i++) {
            if(write(p1[1], msg, n) < n) {
                fprintf(2, "write error\n");
                exit(1);
            }
            if(read(p2[0], msg, n)  < n) {
                fprintf(2, "read error\n");
                exit(1);
            }
        }
        end = uptime();
        fprintf(1, "ping-pong: %d ticks \n", end - start);

        close(p1[1]);
        close(p2[0]);
    }

    exit(0);
}
