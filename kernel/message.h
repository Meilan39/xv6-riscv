#ifndef MESSAGE
#define MESSAGE

#include "defs.h"
#include "types.h"
#include "spinlock.h"

#define MSG_BUF_SIZE 256

struct msg_buffer {
  struct spinlock lock;
  char buf[MSG_BUF_SIZE];
};
extern struct msg_buffer msg;

void msginit(void);

#endif // MESSAGE