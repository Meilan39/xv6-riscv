#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main() {
  char buf1[1];
  char buf1024[1024];
  
  // Create a file with 1024 bytes of content to read from
  int fd_create = open("test_read", O_CREATE | O_RDWR);
  write(fd_create, buf1024, 1024);
  close(fd_create);
  
  int s1 = uptime();
  for (int e = 0; e < 100; e++) {
    int fd = open("test_read", O_RDONLY);
    for(int i = 0; i < 1024; i++) {
      read(fd, buf1, 1);
    }
    close(fd);
  }
  int e1 = uptime();
  
  int s2 = uptime();
  for (int e = 0; e < 100; e++) {
    int fd = open("test_read", O_RDONLY);
    read(fd, buf1024, 1024);
    close(fd);
  }
  int e2 = uptime();
  
  unlink("test_read");
  
  printf("Small reads: %d ticks\n", e1 - s1);
  printf("Large reads: %d ticks\n", e2 - s2);
  
  exit(0);
}