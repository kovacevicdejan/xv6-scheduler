#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "global.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
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

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
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

//set algorithm type for scheduler
//and additional parameters if needed
uint64
sys_set_sch_alg(void)
{
    int at;

    if(argint(0, &at) < 0)
        return -1;

    if(at == SJF) {
        int ip, a;

        if(argint(1, &ip) < 0)
            return -2;

        if(argint(2, &a) < 0)
            return -3;

        if((ip != 0 && ip != 1) || a < 0 || a > 100)
            return -4;

        enum algtype old_type = alg_type;
        alg_type = SJF;
        is_preemptive = ip;
        alpha = a;

        if(old_type == CFS)
            fix_heap();
    }
    else if(at == CFS) {
        enum algtype old_type = alg_type;
        alg_type = CFS;

        if(old_type == CFS)
            fix_heap();
    }
    else return -5;

    return 0;
}
