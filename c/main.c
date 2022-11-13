#include "log.h"
#include "queue.h"
#include "rbtree.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>

#define N 32

char* newStr(int size) {
    return (char*) malloc(size);
}

void freeStr(char* str) {
    free(str);
}

int comp(void* a, void* b) {
    return strcmp((char*) a, (char*) b);
}

int main(int argc, char* argv[]) {
    char* buf;
    logSetLevel(LOG_LEVEL_DEBUG);
    logInfo("testing queue");
    Queue queue = queueInit(32);
    int i=0;
    for(; i<4; i++) {
        buf = newStr(N);
        sprintf(buf, "stringa %d", i);
        queueEnqueue(queue, buf);
    }
    for(int j=0; j<2; j++) {
        freeStr(queueDequeue(queue));
    }
    for(; i<20; i++) {
        buf = newStr(N);
        sprintf(buf, "stringa %d", i);
        queueEnqueue(queue, buf);
    }
    while(queuePeek(queue)){
        printf("%s\n", (char*)queuePeek(queue));
        freeStr(queueDequeue(queue));
    }
    queueDestroy(queue);
    logInfo("testing trees");

    Tree tree = treeInit(comp,N);
    for(int i=0; i<100; i++) {
        buf = newStr(N);
        sprintf(buf, "string %d", i);
        treeInsert(tree, buf);
    }
    treeRemove(tree, "string 4\0");
    buf = newStr(N);
    for(int i=0; i<100; i++) {
        sprintf(buf, "string %d",i);
        if(!treeFind(tree, buf)){
            logError("Expected %s, not found", buf);
            return 1;
        } else {
            freeStr(treeRemove(tree, buf));
        }
    }
    treeDestroy(tree);
    logInfo("End of program");
}
