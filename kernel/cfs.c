#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "global.h"

void put_cfs(struct proc *p) {
    if(p != 0) {
        p->wait_begin = ticks;
        insert(sched_heap, &sched_heap_size, p);
    }
}

struct proc* get_cfs() {
    struct proc* p = delete_min(sched_heap, &sched_heap_size);

    if(p != 0) {
        p->time_slice = (ticks - p->wait_begin) / proc_number;

        if(p->time_slice == 0)
            p->time_slice = 1;

        p->counter = p->time_slice;
    }

    return p;
}
