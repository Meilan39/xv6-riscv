#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "semaphore.h"

struct semaphore semaphores[NSEM];
uint sem_idx = 0;

uint
sem_alloc()
{
  uint idx = sem_idx;
  if(idx >= NSEM)
    panic("semaphore alloc");
  sem_idx++;
  return idx;
}

void
sem_free(uint idx)
{
  if(sem_idx > 0)
    sem_idx--;
}

void
sem_init(uint idx, uint value)
{
  if(idx >= NSEM)
    return;
  struct semaphore *sem = &semaphores[idx];
  initlock(&sem->lk, "semaphore");
  sem->resource = value;
  sem->waiting = 0;
}

/* sleep until resource is avaliable */
void
sem_wait(uint idx)
{
  if(idx >= NSEM)
    return;
  struct semaphore *sem = &semaphores[idx];
  acquire(&sem->lk);
  sem->waiting++;
  while(sem->resource == 0)
    sleep(sem, &sem->lk);
  sem->resource--;
  sem->waiting--;
  release(&sem->lk);
}

/* return resource and wake up a waiting process */
void
sem_post(uint idx) 
{
  if(idx >= NSEM)
    return;
  struct semaphore *sem = &semaphores[idx];
  acquire(&sem->lk);
  sem->resource++;
  release(&sem->lk);
  wakeup(sem);
}

/* return the number of waiting processes */
uint
sem_waiting(uint idx)
{
  if(idx >= NSEM)
    return -1;
  struct semaphore *sem = &semaphores[idx];
  acquire(&sem->lk);
  uint waiting = sem->waiting;
  release(&sem->lk);
  return waiting;
}
