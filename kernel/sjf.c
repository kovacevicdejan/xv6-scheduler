#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "global.h"

int is_preemptive = 1;
int alpha = 50;

void put_sjf(struct proc *p) {
    if(p != 0)
        insert(sched_heap, &sched_heap_size, p);
}

struct proc* get_sjf() {
    struct proc* p = delete_min(sched_heap, &sched_heap_size);

    if(p != 0)
        p->counter = p->time_slice;

    return p;
}
