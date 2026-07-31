#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define BSIZE 1024

int main() {
  char buf[BSIZE];
  memset(buf, 'a', BSIZE);

  int fd = open("file20", O_CREATE | O_RDWR);
  for(int i = 0; i < 20; i++) {
    write(fd, buf, BSIZE);
  }
  close(fd);

  fd = open("file40", O_CREATE | O_RDWR);
  for(int i = 0; i < 40; i++) {
    write(fd, buf, BSIZE);
  }
  close(fd);

  int s1 = uptime();
  int fd1 = open("file20", O_RDONLY);
  for(int e = 0; e < 100000; e++) {
    if(read(fd1, buf, BSIZE) == 0) {
      close(fd1);
      fd1 = open("file20", O_RDONLY);
      read(fd1, buf, BSIZE);
    }
  }
  close(fd1);
  int e1 = uptime();

  int s2 = uptime();
  int fd2 = open("file40", O_RDONLY);
  for(int e = 0; e < 100000; e++) {
    if(read(fd2, buf, BSIZE) == 0) {
      close(fd2);
      fd2 = open("file40", O_RDONLY);
      close(fd2);
      fd2 = open("file40", O_RDONLY);
      read(fd2, buf, BSIZE);
    }
  }
  close(fd2);
  int e2 = uptime();

  unlink("file20");
  unlink("file40");
  
  printf("File20 (Hit): %d ticks\n", e1 - s1);
  printf("File40 (Thrash): %d ticks\n", e2 - s2);
}
