#include "log.h"
#include "rbtree.h"

int sorting_num_p(void* a, void* b) {
    return (*(int*)a) - (*(int*)b);
}
int sorting_num_n(void* a, void* b) {
    return (*(int*)b) - (*(int*)a);
}

int main(int argc, char* argv[]) {
    logSetLevel(LOG_LEVEL_INFO);
    Tree tree = treeInit(sorting_num_p, 1);
    for(int i=0; i<0x7f; i++) {
        treeInsert(tree, &i);
    }
    logWarning("Rebuilding tree...");
    treeRebuild(tree, sorting_num_n);
    treeDestroy(tree);
}