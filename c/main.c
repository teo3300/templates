#include "log.h"
#include "queue.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>

#define N 32

char* str(int n){
    return malloc(n);
}

int main(int argc, char* argv[]) {
    logSetLevel(LOG_LEVEL_WARNING);
    Queue queue = queueInit(N);
    char* buf;
    int i=0;
    for(; i<4; i++) {
        buf = str(N);
        sprintf(buf, "stringa %d", i);
        queueEnqueue(queue, buf);
    }
    for(int j=0; j<2; j++) {
        free(queueDequeue(queue));
    }
    for(; i<20; i++) {
        buf = str(N);
        sprintf(buf, "stringa %d", i);
        queueEnqueue(queue, buf);
    }
    while(queuePeek(queue)){
        printf("%s\n", (char*)queuePeek(queue));
        free(queueDequeue(queue));
    }
    queueDestroy(queue);
    logInfo("End of program");
}
