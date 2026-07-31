#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char symbols[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";

void make_path(char *path, int size, int i) {
  path[size] = '/';
  path[size + 1] = symbols[i];
  path[size + 2] = '\0';
}

void delete_path(char *path, int size) {
  path[size] = '\0';
}

void make_tree(char *path, int n, int target_depth, int depth) {
  if (depth == target_depth) {
    int fd = open(path, O_CREATE | O_RDWR);
    if(fd >= 0) close(fd);
    return;
  }
  
  mkdir(path);
  for (int i = 0; i < n; i++) {
    int size = strlen(path);
    make_path(path, size, i);
    make_tree(path, n, target_depth, depth + 1);
    delete_path(path, size);
  }
}

void access_tree(char *path, int n, int target_depth, int depth) {
  if (depth == target_depth) {
    int fd = open(path, O_RDONLY);
    if(fd >= 0) close(fd);
    return;
  }
  
  for (int i = 0; i < n; i++) {
    int size = strlen(path);
    make_path(path, size, i);
    access_tree(path, n, target_depth, depth + 1);
    delete_path(path, size);
  }
}

void delete_tree(char *path, int n, int target_depth, int depth) {
  if (depth == target_depth) {
    unlink(path);
    return;
  }
  
  for (int i = 0; i < n; i++) {
    int size = strlen(path);
    make_path(path, size, i);
    delete_tree(path, n, target_depth, depth + 1);
    delete_path(path, size);
  }
  unlink(path);
}

int main(int argc, char *argv[]) {
  int n[]     = {2, 4, 8, 64};
  int depth[] = {6, 3, 2, 1};
  
  for (int e = 0; e < 4; e++) {
    char path[256] = "a";

    make_tree(path, n[e], depth[e], 0);
  
    int start = uptime();
    for (int i = 0; i < 100; i++)
      access_tree(path, n[e], depth[e], 0);
    int end = uptime();
    
    printf("[N=%d]: %d ticks\n", n[e], end - start);
    
    delete_tree(path, n[e], depth[e], 0);
  }
}
