#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "queue.h"

typedef struct item{
    struct item *next;
    void *data;
} item;

typedef struct thread_item{
    struct thread_item *next;
    void *data;
    cnd_t curr_cv;
} thread_item;

typedef struct Queue{
    item *first;
    item *last;
    int size;
    mtx_t lock;
    cnd_t not_empty_cv;
} Queue;

typedef struct Thread_Queue{
    thread_item *first;
    thread_item *last;
    int size;
    mtx_t lock;
    cnd_t not_empty_cv;
} Thread_Queue;

Queue queue;
Thread_Queue waiting_queue;
atomic_int waiting_count;
atomic_int visited_count;

void initQueue(){
    // initialize main queue
    mtx_init(&queue.lock, mtx_plain);
    cnd_init(&queue.not_empty_cv);
    queue.first = NULL;
    queue.last = NULL;
    queue.size = 0;

    // initialize waiting threads queue
    mtx_init(&waiting_queue.lock, mtx_plain);
    cnd_init(&waiting_queue.not_empty_cv);
    waiting_queue.first = NULL;
    waiting_queue.last = NULL;
    waiting_queue.size = 0;

    // initialize counters
    waiting_count = 0;
    visited_count = 0;
}
void destroyQueue(){
    // Destroy main queue
    while (queue.size > 0){
        dequeue();
    }
    mtx_destroy(&queue.lock);
    cnd_destroy(&queue.not_empty_cv);

    // Destroy waiting queue
    while (waiting_queue.size > 0) {
        thread_item *next = waiting_queue.first->next;
        cnd_destroy(&waiting_queue.first->curr_cv);
        free(waiting_queue.first->data);
        free(waiting_queue.first);
        waiting_queue.first = next;
        waiting_queue.size--;
    }
    mtx_destroy(&waiting_queue.lock);
    cnd_destroy(&waiting_queue.not_empty_cv);
}

void enqueue(void* data){
    bool updated_waiting = false;
    cnd_t* curr_cv;

    // if there are waiting threads, wake up the first one that arrived
    mtx_lock(&waiting_queue.lock);
    if (waiting_queue.size > 0) {
        curr_cv = &(waiting_queue.first->curr_cv);
        waiting_queue.first->data = data;
        waiting_queue.first = waiting_queue.first->next;
        waiting_queue.size--;
        updated_waiting = true;
    }
    mtx_unlock(&waiting_queue.lock);

    if (updated_waiting){
        cnd_signal(curr_cv);
        return;
    }

    item *newItem = (item *)malloc(sizeof(item));
    newItem->data = data;
    newItem->next = NULL;

    mtx_lock(&queue.lock);
    if (queue.size > 0){
        queue.last->next = newItem;
        queue.last = newItem;
    }
    else{
        queue.first = newItem;
        queue.last = newItem;
    }
    queue.size++;
    mtx_unlock(&queue.lock);
}

void* dequeue(){
    mtx_lock(&queue.lock);
    if (queue.first == NULL) {
        // Create a new condition variable for this thread
        thread_item *newThreadItem = (thread_item *)malloc(sizeof(thread_item));
        cnd_init(&(newThreadItem->curr_cv));
        newThreadItem->next = NULL;

        mtx_lock(&waiting_queue.lock);
        if (waiting_queue.size > 0) {
            waiting_queue.last->next = newThreadItem;
            waiting_queue.last = newThreadItem;
        } else {
            waiting_queue.first = newThreadItem;
            waiting_queue.last = newThreadItem;
        }
        waiting_queue.size++;
        waiting_count++;
        mtx_unlock(&waiting_queue.lock);


        // Wait on the condition variable
        cnd_wait(&(newThreadItem->curr_cv), &queue.lock);


        // Check if there's data in newThreadItem->data
        while (newThreadItem->data == NULL) {
            cnd_wait(&newThreadItem->curr_cv, &queue.lock);
        }

        
        void *data = newThreadItem->data;
        cnd_destroy(&newThreadItem->curr_cv);
        free(newThreadItem);
        visited_count++;
        mtx_unlock(&queue.lock); // Unlock before returning to prevent deadlocks
        waiting_count--;
        return data;
    }

    item *first = queue.first;
    queue.first = queue.first->next;
    queue.size--;
    visited_count++;
    mtx_unlock(&queue.lock);

    void *data = first->data;
    free(first);
    return data;
}


bool tryDequeue(void** pointer_to_data){
    mtx_lock(&queue.lock);
    if (queue.first == NULL) {
        mtx_unlock(&queue.lock);
        return false;
    }
    item *first = queue.first;
    queue.first = queue.first->next;
    queue.size--;
    mtx_unlock(&queue.lock);
    *pointer_to_data = first->data;
    visited_count++;
    return true;
}

size_t size(){
    int size;
    mtx_lock(&queue.lock);
    size = queue.size;
    mtx_unlock(&queue.lock);
    return size;
}
size_t waiting(){
    return waiting_count;
}
size_t visited(){
    return visited_count;
}