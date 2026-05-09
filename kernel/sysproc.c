#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "proc.h"
#include "vm.h"
#include "message.h"
#include "fs.h"
#include "file.h"
#include "pstat.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

/* replica of getpid() */
uint64
sys_my_getpid(void) {
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    if(addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_check_proc(void) {
  int pid;
  argint(0, &pid);
  return checkproc(pid);
}

uint64
sys_get_msg(void) {
  uint64 buf_ptr; // buffer to write to
  int buf_size;   // maximum bytes to write
  argaddr(0, &buf_ptr);
  argint(1, &buf_size);

  if(buf_size <= 0 || buf_size > MSG_BUF_SIZE) {
    return -1; // error: invalid buffer size
  }
  
  acquire(&msg.lock);
  if(copyout(myproc()->pagetable, buf_ptr, msg.buf, buf_size) == -1)
    buf_size = -1; // return with error
  release(&msg.lock);

  return buf_size; // return number of bytes read
}

uint64
sys_set_msg(void) {
  uint64 buf_ptr;   // buffer to read from
  int buf_size;     // number of bytes to read
  argaddr(0, &buf_ptr);
  argint(1, &buf_size);

  if(buf_size <= 0 || buf_size > MSG_BUF_SIZE) {
    return -1; // error: invalid buffer size
  }

  acquire(&msg.lock);
  if(copyin(myproc()->pagetable, msg.buf, buf_ptr, buf_size) == -1)
    buf_size = -1; // return with error
  release(&msg.lock);

  return buf_size; // return number of bytes read
}

uint64
sys_get_pgdir(void) {
  return (uint64)myproc()->pagetable;
}

uint64
sys_validpg_num(void) {
  return validpg_num(myproc()->pagetable);
}

uint64
getcwd_helper(struct inode *curr, char *buffer, int *idx)
{
  if(curr->inum == ROOTINO) {
    buffer[(*idx)++] = '~'; 
    buffer[(*idx)++] = '/'; 
    return 0;
  }
  
  ilock(curr);
  struct inode *parent = dirlookup(curr, "..", 0);
  iunlock(curr);
  if(parent == 0) return -1;

  struct dirent de;
  int found = 0;
  ilock(parent);
  for(uint off = 0; off < parent->size; off += sizeof(de)){
    if(readi(parent, 0, (uint64)&de, off, sizeof(de)) != sizeof(de)) 
      break;
    if(de.inum == curr->inum){
      found = 1;
      break;
    }
  }
  iunlock(parent);

  if(!found){
    iput(parent);
    return -1;
  }

  if(getcwd_helper(parent, buffer, idx) < 0){
    iput(parent);
    return -1;
  }

  if(parent->inum != ROOTINO) 
    buffer[(*idx)++] = '/'; 
  int len = strlen(de.name);
  memmove(&buffer[*idx], de.name, len);
  *idx += len;

  iput(parent);
  return 0;
}

uint64
sys_getcwd(void) {
  char buffer[256];
  int err = 0, idx = 0;
  struct inode *cwd = myproc()->cwd;
  uint64 buf_ptr;
  int buf_size;

  argaddr(0, &buf_ptr);
  argint(1, &buf_size);

  begin_op();
  err = getcwd_helper(cwd, buffer, &idx);
  end_op();
  buffer[idx++] = '\0';

  if(err == -1)
    return -1;
  if(idx > buf_size)
    return -1;
  if(copyout(myproc()->pagetable, buf_ptr, buffer, idx) == -1)
    return -1;
  return idx;
}

uint64
sys_settickets(void) {
  int ticket;
  argint(0, &ticket);

  if(ticket < 10 || 150 < ticket || ticket % 10)
    return -1;

  struct proc *p = myproc();
  acquire(&p->lock);
  p->ticket = ticket;
  p->stride = STRIDE_DIVISOR / p->ticket;
  release(&p->lock);

  return 0;
}

uint64
sys_getpinfo(void) {
  uint64 addr;
  argaddr(0, &addr);

  struct pstat kp;
  getpinfo(&kp);

  if(copyout(myproc()->pagetable, addr, (char *)&kp, sizeof(kp)) < 0)
    return -1;
  return 0;  
}

uint64
sys_clone(void) {
  return kclone();
}

uint64
sys_sem_alloc(void)
{
  return sem_alloc();
}

uint64
sys_sem_free(void)
{
  uint idx;
  argint(0, (int*)&idx);
  sem_free(idx);
  return 0;
}

uint64
sys_sem_init(void)
{
  uint idx;
  uint value;
  
  argint(0, (int*)&idx);
  argint(1, (int*)&value);
  
  sem_init(idx, value);
  return 0;
}

uint64
sys_sem_wait(void)
{
  uint idx;
  argint(0, (int*)&idx);
  sem_wait(idx);
  return 0;
}

uint64
sys_sem_post(void)
{
  uint idx;
  argint(0, (int*)&idx);
  sem_post(idx);
  return 0;
}

uint64
sys_sem_waiting(void)
{
  uint idx;
  argint(0, (int*)&idx);
  return sem_waiting(idx);
}