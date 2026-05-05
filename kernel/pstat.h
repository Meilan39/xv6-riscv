#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

struct pstat {
  int inuse[NPROC];
  int pid[NPROC];
  int ticket[NPROC];
  int stride[NPROC];
  int pass[NPROC];
};

#endif // _PSTAT_H_
