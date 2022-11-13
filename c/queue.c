#include "queue.h"
#include <malloc.h>
#include <string.h>
#include "log.h"

#define QUEUE_DEFAULT_SIZE 64

#ifndef QUEUE_INITIAL_SIZE
#define QUEUE_INITIAL_SIZE (QUEUE_DEFAULT_SIZE)
#endif

typedef struct QUEUE_T {
    void** data;
    unsigned int first;
    unsigned int length;
    unsigned int size;
    unsigned int data_size;
} queue_t;
typedef queue_t *queue_ptr;

queue_ptr queueInit(unsigned int data_size) {
    queue_ptr tmp = (queue_ptr)malloc(QUEUE_INITIAL_SIZE*sizeof(queue_t));
        if(!tmp) { goto bad_str_alloc; }
    tmp->data = (void**)malloc(QUEUE_INITIAL_SIZE*sizeof(void*));
        if(!tmp) { goto bad_vec_alloc; }
    tmp->first = 0;
    tmp->length = 0;
    tmp->size = QUEUE_INITIAL_SIZE;
    tmp->data_size = data_size;

    return tmp;

    bad_vec_alloc:
        free(tmp);
    bad_str_alloc:
        logError("Unable to create queue");
        return NULL;
}
unsigned int queueLen(queue_ptr queue) {
    return queue->length;
}

void* queuePeek(queue_ptr queue) {
    // peek first element in queue
    if(queueLen(queue) == 0) return NULL;
    return queue->data[queue->first];
}

void* queueDequeue(queue_ptr queue) {
    void* to_dequeue = queuePeek(queue);
    if (to_dequeue == NULL) return NULL;

    queue->first = (queue->first + 1)%(queue->size);
    queue->length--;

    logDebug("Element %p removed from queue %p (%d)", to_dequeue, queue, queue->first);
    return to_dequeue;
}

void* queueEnqueue(queue_ptr queue, void* data) {
    unsigned int new_size = queue->size*2;
    void** new_data;
    if(queue->length >= queue->size) {
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
}
