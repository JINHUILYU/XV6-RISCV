#ifndef _PROCINFO_H_
#define _PROCINFO_H_

#define PROC_NAME_LEN 16

struct uproc {
  int pid;
  int ppid;
  int state;
  int killed;
  uint64 sz;
  char name[PROC_NAME_LEN];
};

#endif