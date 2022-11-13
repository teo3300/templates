#include "heap.h"
#include "log.h"
#include <malloc.h>

#define HEAP_DEFAULT_SIZE 64

#ifndef HEAP_INITIAL_SIZE
#define HEAP_INITIAL_SIZE (HEAP_DEFAULT_SIZE)
#endif//HEAP_INITIAL_SIZE

typedef struct HEAP_T{
    void      **data;
    int       (*sorting_criteria)(void*, void*);
    unsigned int        data_size;
    unsigned int        length;
    unsigned int        size;
} heap_t;
typedef heap_t *heap_ptr;
typedef heap_ptr Heap;

#define PARENT(i)       ((i)/2)
#define LEFT(i)         ((i)*2)
#define RIGHT(i)        ((i)*2+1)
#define heapTop(heap)   (heap->data[1])
#define SWAP(a,b)       {void* C = heap->data[a]; heap->data[a] = heap->data[b]; heap->data[b] = C;}

unsigned int heapSize(heap_ptr heap) {
    return heap->length;
}

heap_ptr heapInit(int(*sorting_criteria)(void*, void*), unsigned int data_size){
    heap_ptr tmp = (heap_ptr)malloc(sizeof(heap_t));
        if(!tmp) { goto bad_str_alloc; }
    tmp->data = (void**)malloc(HEAP_INITIAL_SIZE*sizeof(void*));
        if(!tmp->data) { goto bad_vec_alloc; }
    tmp->data_size = data_size;
    tmp->size = HEAP_INITIAL_SIZE;
    tmp->length = 0;
    tmp->sorting_criteria = sorting_criteria;

    return tmp;

    bad_vec_alloc:
        free(tmp);
    bad_str_alloc:
        return NULL;
}

void heapify(heap_ptr heap, unsigned int i){
    unsigned int l = LEFT(i);
    unsigned int r = RIGHT(i);
    unsigned int top;
    if(l <= heap->length && heap->sorting_criteria(heap->data[l], heap->data[i]) > 0) {
        top = l;
    } else top = i;
    if(r <= heap->length && heap->sorting_criteria(heap->data[r], heap->data[top]) > 0) {
        top = r;
    }
    if(top != i){
        SWAP(i, top);
        heapify(heap, top);
    }
}

void heapUpdate(heap_ptr heap, unsigned int i){
    while(i > 1 && heap->sorting_criteria(heap->data[i], heap->data[PARENT(i)]) > 0){
        SWAP(i, PARENT(i));
        i = PARENT(i);
    }

}

void* heapOOOInsert(Heap heap, void* data){
    if(heap->length == heap->size) {
        unsigned int new_size  = heap->size + HEAP_INITIAL_SIZE;
        void** new_data = (void**)realloc(heap->data, new_size*sizeof(void*));
        if(!new_data) return NULL;
        logDebug("Heap realloc");
        heap->size = new_size;
    };
    heap->data[++heap->length] = data;
    return heap->data[heap->length];
}

void heapBuild(Heap heap){
    for(unsigned int i=PARENT(heap->length); i>0; i--)
        heapify(heap, i);
}

void heapRebuild(Heap heap, int (*sorting_criteria)(void*, void*)){
    heap->sorting_criteria = sorting_criteria;
    heapBuild(heap);
}

void* heapInsert(Heap heap, void* data){
    void* ret = heapOOOInsert(heap, data);
    if(! ret) return 0;
    heapUpdate(heap, heap->length);
    return ret;
}

void* heapExtract(Heap heap){
    if (! heap->length) return NULL;
    void* ret;
    ret = heapTop(heap);
    heapTop(heap) = heap->data[heap->length];
    heap->length--;
    heapify(heap, 1);
    return ret;
}

void heapDestroy(Heap heap){
    free(heap->data);
    free(heap);
}