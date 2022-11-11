#include "log.h"
#include "queue.h"
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

int main(int argc, char* argv[]) {
    char* buf;
    logSetLevel(LOG_LEVEL_WARNING);
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
    logInfo("End of program");
}
