#include "queue.h"
#include "log.h"
#include "rbtree.h"
#include <malloc.h>
#include <string.h>

#ifndef QUEUE_DEFAULT_SIZE
#define QUEUE_DEFAULT_SIZE 8
#endif//QUEUE_DEFAULT_SIZE

#define QUEUE_INITIAL_SIZE (QUEUE_DEFAULT_SIZE)

typedef struct QUEUE_T {
    void** data;
    counter_t first;
    counter_t length;
    counter_t size;
    counter_t data_size;
} queue_t;
typedef queue_t *queue_ptr;

queue_ptr queueInit(counter_t data_size) {
    // create queue struct
    queue_ptr tmp = (queue_ptr)malloc(sizeof(queue_t));
    if(!tmp) { goto bad_str_alloc; }

    // create queue data vector
    tmp->data = (void**)malloc(QUEUE_INITIAL_SIZE*sizeof(void*));
    if(!tmp) { goto bad_vec_alloc; }

    // initialize queue
    tmp->first = 0;
    tmp->length = 0;
    tmp->size = QUEUE_INITIAL_SIZE;
    tmp->data_size = data_size;
    logInfo("New queue created at %p", tmp);

    return tmp;

    bad_vec_alloc:
        free(tmp);
    bad_str_alloc:
        logError("Unable to create queue");
        return NULL;
}
counter_t queueLen(queue_ptr queue) {
    // get queue length
    return queue->length;
}

void* queuePeek(queue_ptr queue) {
    // peek first element in queue
    if(queueLen(queue) == 0) return NULL;
    return queue->data[queue->first];
}

void* queueDequeue(queue_ptr queue) {
    // get return value
    void* to_dequeue = queuePeek(queue);
    if (to_dequeue == NULL) return NULL;

    // move queue pointers
    queue->first = (queue->first + 1)%(queue->size);
    queue->length--;

    logDebug("Element %p removed from queue %p (%d)", to_dequeue, queue, queue->first);
    return to_dequeue;
}

void* queueEnqueue(queue_ptr queue, void* data) {
    // the queue need to be resized
    if(queue->length >= queue->size) {
        void** new_data;
        // compute new queue size
        counter_t new_size = queue->size+QUEUE_INITIAL_SIZE;
        logInfo("Enlarging queue to %d", new_size);
        new_data = (void**)malloc(sizeof(void*)*new_size);
            if(!new_data) { goto bad_queue_realloc; }

        // split memcpy
        memcpy(&new_data[queue->size-queue->first], queue->data, sizeof(void*)*queue->first);
        memcpy(new_data, &queue->data[queue->first], sizeof(void*)*(queue->size-queue->first));

        // free old queue
        free(queue->data);
        queue->first = 0;
        queue->data = new_data;
        queue->size = new_size;
    }
    logDebug("Adding element %p to queue %p in position %d(%d+%d)", data, queue, (queue->first+queue->length)%(queue->size), queue->first, queue->length);
    queue->data[(queue->first+queue->length)%(queue->size)] = data;
    queue->length++;
    
    return data;
    bad_queue_realloc:
        logWarning("Bad data reallocation in queue enlargment, not adding more data");
        return NULL;
}

void queueDestroy(queue_ptr queue){
    free(queue->data);
    free(queue);
    logInfo("Queue struct %p removed", queue);
}
