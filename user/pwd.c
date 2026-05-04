#include "kernel/types.h"
#include "user.h"

#define PATH_SIZE 256

int main(int argc, char *argv[]){
  char buffer[PATH_SIZE];
  if(getcwd(buffer, PATH_SIZE) < 0){
    printf("pwd failed\n");
    exit(-1);
  }
  printf("%s\n", buffer);
  exit(0);
}