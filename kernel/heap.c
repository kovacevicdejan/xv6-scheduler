#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "global.h"

void swap(struct proc** proc_a, struct proc** proc_b) {
    struct proc* tmp = *proc_a;
    *proc_a = *proc_b;
    *proc_b = tmp;
}

int get_value(struct proc* p) {
    if(alg_type == SJF)
        return p->tau;
    else
        return p->last_burst;
}

int get_parent(int index) {
    return (index - 1) / 2;
}

int get_left_child(int index) {
    return index * 2 + 1;
}

int get_right_child(int index) {
    return (index + 1) * 2;
}

void move_up(struct proc** heap, int index) {
    int curr = index;
    int parent = get_parent(curr);

    while(curr != 0 && get_value(heap[curr]) < get_value(heap[parent])) {
        swap(&heap[curr], &heap[parent]);
        curr = parent;
        parent = get_parent(curr);
    }
}

void move_down(struct proc** heap, int heap_size, int index) {
    int curr = index;
    int left = get_left_child(curr);
    int right = get_right_child(curr);

    while((left < heap_size && get_value(heap[curr]) > get_value(heap[left]))
        || (right < heap_size && get_value(heap[curr]) > get_value(heap[right]))) {
        int min = (right >= heap_size || get_value(heap[left]) < get_value(heap[right])) ? left : right;
        swap(&heap[curr], &heap[min]);
        curr = min;
        left = get_left_child(curr);
        right = get_right_child(curr);
    }
}

struct proc* get_min(struct proc** heap, int heap_size) {
    acquire(&heap_lock);

    if(heap_size > 0) {
        release(&heap_lock);
        return heap[0];
    }
    else {
        release(&heap_lock);
        return 0;
    }
}

void insert(struct proc** heap, int* heap_size, struct proc* p) {
    if(*heap_size == 0) {
        heap[0] = p;
        *heap_size = 1;
    }
    else {
        int curr = (*heap_size)++;
        heap[curr] = p;
        move_up(heap, curr);
    }
}

struct proc* delete_min(struct proc** heap, int* heap_size) {
    if(*heap_size == 0)
        return 0;

    struct proc* p = heap[0];
    heap[0] = heap[--(*heap_size)];
    move_down(heap, *heap_size, 0);

    return p;
}

void heapify(struct proc** heap, int heap_size) {
    int i;

    for(i = heap_size - 1; i >= 0; --i) {
        move_down(heap, heap_size, i);
    }
}
