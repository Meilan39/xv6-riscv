#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
#include "message.h"

/* singleton */
struct msg_buffer msg;

void msginit() {
  initlock(&msg.lock, "message");
}