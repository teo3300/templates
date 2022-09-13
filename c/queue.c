#include "queue.h"
#include "log.h"
#include "rbtree.h"
#include <malloc.h>
#include <string.h>

#ifndef QUEUE_INITIAL_SIZE
#define QUEUE_INITIAL_SIZE (8)
#endif

#ifndef QUEUE_MAXIMUM_SIZE
#define QUEUE_MAXIMUM_SIZE (QUEUE_INITIAL_SIZE << 10)
#endif

typedef struct QUEUE_T {
    void** data;
    counter_t first;
    counter_t length;
    counter_t size;
    counter_t data_size;
    counter_t mask;
} queue_t;
typedef queue_t *queue_ptr;

queue_ptr queueInit(counter_t data_size) {
    queue_ptr tmp = (queue_ptr)malloc(sizeof(queue_t));
        if(!tmp) { logError("Unable to alloc queue structure"); goto bad_str_alloc; }
    tmp->data = (void**)malloc(QUEUE_INITIAL_SIZE*sizeof(void*));
        if(!tmp) { logError("Unable to alloc queue vector"); goto bad_vec_alloc; }
    tmp->first = 0;
    tmp->length = 0;
    tmp->size = QUEUE_INITIAL_SIZE;
    tmp->data_size = data_size;
    tmp->mask = QUEUE_INITIAL_SIZE-1;
    logInfo("New queue created at %p", tmp);

    return tmp;

    bad_vec_alloc:
        free(tmp);
    bad_str_alloc:
        return NULL;
}
counter_t queueLen(queue_ptr queue) {
    return queue->length;
}

void* queuePeek(queue_ptr queue) {
    if (queue->length == 0) return NULL;
    return queue->data[queue->first];
}

void* queueDequeue(queue_ptr queue) {
    void* to_delete = queuePeek(queue);
    if (to_delete == NULL) return NULL;
    logDebug("Removing element %p from queue %p (%d)", to_delete, queue, queue->first);

    #if LIBALLOC
        free(to_delete);
    #endif
    queue->first = (queue->first + 1)&(queue->mask);
    queue->length--;

    return to_delete;
}

void* queueEnqueue(queue_ptr queue, void* data) {
    counter_t new_size = queue->size*2;
    void** new_data;
    if(queue->length >= queue->size) {
        if(new_size > QUEUE_MAXIMUM_SIZE) {
            logWarning("Queue maximum size reached, not adding any more data");
            return NULL;
        }
        logInfo("Enlarging queue to %d", new_size);
        new_data = (void**)malloc(sizeof(void*)*new_size);
            if(!new_data) { logWarning("Bad data reallocation in queue enlargment, not adding more data"); goto bad_queue_realloc; }

        // split memcpy
        memcpy(&new_data[queue->size-queue->first], queue->data, sizeof(void*)*queue->first);
        memcpy(new_data, &queue->data[queue->first], sizeof(void*)*(queue->size-queue->first));

        free(queue->data);
        queue->first = 0;
        queue->data = new_data;
        queue->size = new_size;
        queue->mask = new_size-1;
    }
    #if LIBALLOC
        void* tmp = (void*)malloc(queue->data_size); if(!tmp) { logWarning("Bad data alloc during insertion"); goto bad_data_alloc; }
        memcpy(tmp, data, queue->data_size);
        logDebug("Adding element %p to queue %p in position %d(%d+%d)", tmp, queue, (queue->first+queue->length)&(queue->mask), queue->first, queue->length);
        queue->data[(queue->first+queue->length)&(queue->mask)] = tmp;
    #else
        logDebug("Adding element %p to queue %p in position %d(%d+%d)", data, queue, (queue->first+queue->length)&(queue->mask), queue->first, queue->length);
        queue->data[(queue->first+queue->length)&queue->mask] = data;
    #endif
    queue->length++;
    
    return queue;

    bad_data_alloc: bad_queue_realloc:
        return NULL;
}

void queueDestroy(queue_ptr queue){
    #if LIBALLOC
        logInfo("Removing elements fom queue %p", queue);
        while(queueDequeue(queue));
    #endif
    free(queue->data);
    free(queue);
    logInfo("Queue struct %p removed", queue);
}