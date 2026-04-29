#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "Usage: mv src dst\n");
    exit(1);
  }

  if(link(argv[1], argv[2]) < 0){
    fprintf(2, "mv: cannot link %s to %s\n", argv[1], argv[2]);
    exit(1);
  }

  if(unlink(argv[1]) < 0){
    fprintf(2, "mv: failed to delete %s\n", argv[1]);
    exit(1);
  }

  exit(0);
}
