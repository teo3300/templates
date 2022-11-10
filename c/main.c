#include "log.h"
#include "queue.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    logSetLevel(LOG_LEVEL_WARNING);
    Queue queue = queueInit(32);
    char buf[32];
    int i=0;
    for(; i<4; i++) {
        sprintf(buf, "stringa %d", i);
        queueEnqueue(queue, buf);
    }
    for(int j=0; j<2; j++) {
        queueDequeue(queue);
    }
    for(; i<20; i++) {
        sprintf(buf, "stringa %d", i);
        queueEnqueue(queue, buf);
    }
    while(queuePeek(queue)){
        printf("%s\n", (char*)queuePeek(queue));
        queueDequeue(queue);
    }
    queueDestroy(queue);
    logInfo("End of program");
}
