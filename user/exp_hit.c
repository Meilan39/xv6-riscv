#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define BSIZE 1024
#define BLOCKS 20 // Must be less than NBUF (30) to fit completely in cache

void create_file(char *name) {
  char buf[BSIZE];
  memset(buf, 'a', BSIZE);
  int fd = open(name, O_CREATE | O_RDWR);
  for (int i = 0; i < BLOCKS; i++) {
    write(fd, buf, BSIZE);
  }
  close(fd);
}

void read_file(char *name, int iters, char *label) {
  char buf[BSIZE];
  int start = uptime();
  for (int i = 0; i < iters; i++) {
    int fd = open(name, O_RDONLY);
    while(read(fd, buf, BSIZE) > 0) {}
    close(fd);
  }
  int end = uptime();
  printf("%s: %d ticks\n", label, end - start);
}

int main() {
  create_file("file1");
  create_file("file2");

  read_file("file1", 50, "Cache Miss");
  read_file("file1", 50, "Cache Hit");

  unlink("file1");
  unlink("file2");
}
