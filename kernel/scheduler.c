#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "global.h"

enum algtype alg_type = SJF;
int proc_number = 0;
struct proc* sched_heap[NPROC];
int sched_heap_size = 0;
struct spinlock heap_lock;

void put(struct proc *p) {
    acquire(&heap_lock);

    if(alg_type == SJF)
        put_sjf(p);
    else
        put_cfs(p);

    release(&heap_lock);
}

struct proc* get() {
    struct proc* p;
    acquire(&heap_lock);

    if(alg_type == SJF)
        p = get_sjf();
    else
        p = get_cfs();

    release(&heap_lock);
    return p;
}

void fix_heap() {
    acquire(&heap_lock);
    heapify(sched_heap, sched_heap_size);
    release(&heap_lock);
}
