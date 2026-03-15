#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(void)
{
  struct procinfo info;
  
  if (procinfo(getpid(), &info) < 0) {
    printf("procinfo failed\n");
    exit(1);
  }
  
  printf("Process: %s\n", info.name);
  printf("PID: %d, PPID: %d\n", info.pid, info.ppid);
  printf("State: %d\n", info.state);
  printf("Memory: %d bytes\n", (int)info.sz);
  
  exit(0);
}
