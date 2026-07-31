#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main() {
  char buf1[1] = {'a'};
  char buf1024[1024];
  memset(buf1024, 'a', 1024);
  
  int s1 = uptime();
  for (int e = 0; e < 100; e++) {
    int fd = open("sm_log", O_CREATE | O_RDWR);
    for(int i = 0; i < 1024; i++) {
      write(fd, buf1, 1);
    }
    close(fd);
  }
  int e1 = uptime();
  
  int s2 = uptime();
  for (int e = 0; e < 100; e++) {
    int fd = open("lg_log", O_CREATE | O_RDWR);
    write(fd, buf1024, 1024);
    close(fd);
  }
  int e2 = uptime();
  
  unlink("sm_log");
  unlink("lg_log");
  
  printf("Small writes: %d ticks\n", e1 - s1);
  printf("Large writes: %d ticks\n", e2 - s2);
}