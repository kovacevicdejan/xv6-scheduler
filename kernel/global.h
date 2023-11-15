#ifndef XV6_OS2_GLOBAL_H
#define XV6_OS2_GLOBAL_H

#include "defs.h"
#include "param.h"

enum algtype{SJF, CFS};
extern enum algtype alg_type;
extern int is_preemptive;
extern int alpha;
extern int proc_number;
extern struct proc* sched_heap[NPROC];
extern int sched_heap_size;
extern struct spinlock heap_lock;

#endif //XV6_OS2_GLOBAL_H
