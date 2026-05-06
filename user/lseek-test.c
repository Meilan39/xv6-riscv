#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int fd = open("lseek_test", O_CREATE|O_RDWR);
  write(fd, "hello word", 10);
  lseek(fd, 6, SEEK_SET);
  write(fd, "world", 5);
  lseek(fd, 0, SEEK_END);
  write(fd, "!", 1);
  close(fd);

  char buf[20];
  fd = open("lseek_test", O_RDONLY);
  int n = read(fd, buf, sizeof(buf) - 1);
  if(n >= 0) buf[n] = '\0';
  printf("%s\n", buf);
  close(fd);

  exit(0);
}