#include "spinlock.h"

#define NSEM 10

struct semaphore {
  uint resource;      // Number of available resources
  struct spinlock lk; // spinlock protecting this sleep lock
  
  /* debug */
  uint waiting;       // Number of waiting processes
};

extern struct semaphore semaphores[NSEM];