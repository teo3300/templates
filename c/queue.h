#ifndef C_QUEUE_H
#define C_QUEUE_H

#ifndef counter_t
#define counter_t unsigned int
#endif

typedef struct QUEUE_T queue_t;
typedef queue_t *queue_ptr;
typedef queue_ptr Queue;

queue_ptr queueInit(counter_t data_size);
void queueDestroy(queue_ptr queue);
counter_t queueLen(queue_ptr queue);
void* queuePeek(queue_ptr queue);
void* queueDequeue(queue_ptr queue);
void* queueEnqueue(queue_ptr queue, void* data);

#endif//C_QUEUE_H
