#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i, fd;
  int e = 0;

  if(argc < 2){
    fprintf(2, "Usage: touch files...\n");
    exit(1);
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], O_CREATE | O_RDWR)) < 0){
      fprintf(2, "touch: %s failed to create\n", argv[i]);
      e = 1;
      continue;
    }
    close(fd);
  }

  exit(e);
}
