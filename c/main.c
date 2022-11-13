#include "log.h"
#include "queue.h"
#include "rbtree.h"
#include "heap.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>

#define N 32

char* newStr(int n) {
    return (char*) malloc(n);
}

int alph(void* a, void* b) {
    return strcmp((char *) a, (char *) b);
}

int main(int argc, char* argv[]) {
    logSetLevel(LOG_LEVEL_DEBUG);

    char* buf;

    Heap heap = heapInit(alph, N);

    for(int i=0; i<20; i++) {
        buf = newStr(N);
        sprintf(buf, "string %03d", i);
        heapOOOInsert(heap, buf);
    }

    heapBuild(heap);
    
    while(heapSize(heap)){
        buf = heapExtract(heap);
        logInfo(buf);
        free(buf);
    }

    heapDestroy(heap);
}