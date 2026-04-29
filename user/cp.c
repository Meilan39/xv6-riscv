#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[1024];

void
cp(int src, int dst)
{
  int n;

  while((n = read(src, buf, sizeof(buf))) > 0) {
    if (write(dst, buf, n) != n) {
      fprintf(2, "cp: write error\n");
      exit(1);
    }
  }
  if(n < 0){
    fprintf(2, "cp: read error\n");
    exit(1);
  }
}

int
main(int argc, char *argv[])
{
  int src, dst;

  if(argc != 3){
    fprintf(2, "Usage: cp src dst\n");
    exit(1);
  }

  if((src = open(argv[1], O_RDONLY)) < 0) {
    fprintf(2, "cp: cannot open %s\n", argv[1]);
    exit(1);
  }

  if((dst = open(argv[2], O_CREATE | O_WRONLY | O_TRUNC)) < 0) {
    fprintf(2, "cp: cannot open %s\n", argv[2]);
    close(src);
    exit(1);
  }
 
  cp(src, dst);

  close(src);
  close(dst);
  exit(0);
}
