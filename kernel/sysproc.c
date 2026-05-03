#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"
#include "message.h"

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