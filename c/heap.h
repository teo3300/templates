#ifndef C_HEAP_H
#define C_HEAP_H

typedef struct HEAP_T heap_t;
typedef heap_t *heap_ptr;
typedef heap_ptr Heap;

heap_ptr heapInit(int(*sorting_criteria)(void*, void*), unsigned int data_size);
unsigned int heapSize(heap_ptr heap);
void heapBuild(Heap heap);
void heapify(heap_ptr heap, unsigned int i);
void heapRebuild(heap_ptr heap, int(*sorting_criteria)(void*, void*));
void* heapOOOInsert(Heap heap, void* data);
void* heapInsert(heap_ptr heap, void* data);
void* heapPeek(heap_ptr heap);
void* heapExtract(Heap heap);
void heapDestroy(heap_ptr heap);

#endif//C_HEAP_H
