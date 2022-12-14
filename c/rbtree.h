#ifndef C_RBTREE_H
#define C_RBTREE_H

typedef struct TREENODE tree_node_t;
typedef tree_node_t *tree_node_ptr;

typedef struct RB_TREE_T rb_tree_t;
typedef rb_tree_t *rb_tree_ptr;
typedef rb_tree_ptr Tree;

rb_tree_ptr treeInit(int(*sorting_criteria)(void*, void*), unsigned int data_size);
unsigned int treeSize(rb_tree_ptr tree);
void* treeMin(rb_tree_ptr tree);
void* treeMax(rb_tree_ptr tree);
void* treeInsert(rb_tree_ptr tree, void *data);
void* treeRemove(rb_tree_ptr tree, void* data);
void  treeRebuild(rb_tree_ptr tree, int(*sorting_criteria)(void*, void*));
void* treeFind(rb_tree_ptr tree, void* data_ptr);
void  treeDestroy(rb_tree_ptr tree);

#endif//C_RBTREE_H
