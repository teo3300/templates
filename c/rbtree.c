#include "rbtree.h"
#include <string.h>
#include <malloc.h>
#include "log.h"

unsigned int tree_counter = 0;
tree_node_ptr global_nil = 0x0;

typedef enum COLOR_T {
    BLACK = 0,
    RED
} color_t;

typedef struct TREENODE {
    struct TREENODE *left,
                    *parent,
                    *right;
    void            *data;
    color_t         color;
} tree_node_t;

typedef struct RB_TREE_T {
    tree_node_ptr   root;
    int             (*sorting_criteria)(void*, void*);
    void            (*data_print)(void*);
    unsigned int       size;
    unsigned int       data_size;
} rb_tree_t;

tree_node_ptr treeMinRel(rb_tree_ptr tree, tree_node_ptr node);
tree_node_ptr treeMaxRel(rb_tree_ptr tree, tree_node_ptr node);
tree_node_ptr treeRemoveLib(rb_tree_ptr tree, tree_node_ptr z);
tree_node_ptr treeFindLib(rb_tree_ptr tree, void* data_ptr);
void treeLeftRotate(rb_tree_ptr tree, tree_node_ptr x);
void treeRightRotate(rb_tree_ptr tree, tree_node_ptr x);
void treeTransplant(rb_tree_ptr tree, tree_node_ptr u, tree_node_ptr v);
tree_node_ptr treeInsertLib(rb_tree_ptr tree, tree_node_ptr z);
tree_node_ptr treeInsertFixup(rb_tree_ptr tree, tree_node_ptr z);
void treeDeleteFixup(rb_tree_ptr tree, tree_node_ptr x);
void treeDestroyRec(rb_tree_ptr tree, tree_node_ptr node);

unsigned int treeSize(rb_tree_ptr tree) { return tree->size; }

rb_tree_ptr treeInit (int(*sorting_criteria)(void*, void*), unsigned int data_size) {
    if(!global_nil) {
        global_nil = (tree_node_ptr)malloc(sizeof(tree_node_t)); if(!global_nil) { logError("Bad common nil allocation"); goto bad_nil_alloc; }
        *global_nil = (tree_node_t){
            .left = global_nil,
            .parent = global_nil,
            .right = global_nil,
            .data = NULL,
            .color = BLACK};
        logInfo("Common nil created");
    }
    rb_tree_ptr tree = (rb_tree_ptr)malloc(sizeof(rb_tree_t)); if(!tree) { logError("Bad tree allocation"); goto bad_tree_alloc; } 
    *tree = (rb_tree_t){
        .root = global_nil,
        .sorting_criteria = sorting_criteria,
        .data_size = data_size,
        .size = 0,
        .data_print = NULL};
    tree_counter++;
    logInfo("New tree created at %p tree_counter: %d", tree, tree_counter);
    return tree;
    
    bad_tree_alloc:
        if(!tree_counter) free(global_nil);
    bad_nil_alloc:
        return NULL;
}

void* treeMin(rb_tree_ptr tree) {
    return treeMinRel(tree, tree->root)->data;
}

void* treeMax(rb_tree_ptr tree) {
    return treeMaxRel(tree, tree->root)->data;
}

tree_node_ptr treeMinRel(rb_tree_ptr tree, tree_node_ptr x) {
    while(x->left != global_nil) {
        x = x->left;
    }
    return x;
}
tree_node_ptr treeMaxRel(rb_tree_ptr tree, tree_node_ptr x) {
    while(x->right != global_nil) {
        x = x->right;
    } return x;
}
tree_node_ptr treeNext(rb_tree_ptr tree, tree_node_ptr x) {
    if (x->right != global_nil){
        return treeMinRel(tree, x->right);
    }
    tree_node_ptr y = x->parent;
    while (y != global_nil && x == y->right){
        x = y;
        y = y->parent;
    }
    return y;
}
tree_node_ptr treePrev(rb_tree_ptr tree, tree_node_ptr x) {
    if (x->left != global_nil){
        return treeMaxRel(tree, x->left);
    }
    tree_node_ptr y = x->parent;
    while (y != global_nil && x == y->left){
        x = y;
        y = y->parent;
    }
    return y;
}
void* treeInsert(rb_tree_ptr tree, void* data){
    tree_node_ptr node = (tree_node_ptr)malloc(sizeof(tree_node_t));
    if(!node) { logError("Bad node alloc during insertion"); goto node_bad_alloc; }

    node->data = data;
    return treeInsertLib(tree, node);

    data_bad_alloc:
        free(node);
    node_bad_alloc:
        return NULL;

}
tree_node_ptr treeRemoveLib(rb_tree_ptr tree, tree_node_ptr z) {
    logDebug("Removing node %p", z);
    if (z == global_nil) return z;
    tree_node_ptr y = z;
    tree_node_ptr x;
    color_t y_original_color = y->color;
    if (z->left == global_nil){
        x = z->right;
        treeTransplant(tree, z, z->right);
    } else if (z->right == global_nil){
        x = z->left;
        treeTransplant(tree, z, z->left);
    } else {
        y = treeMinRel(tree, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z){
            x->parent = y;
        } else {
            treeTransplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        treeTransplant(tree, z, y);     // that was bad
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == BLACK){
        treeDeleteFixup(tree, x);
    }
    tree->size--;
    logDebug("Removed");
    return z;
}
void* treeRemove(rb_tree_ptr tree, void *data) {
    tree_node_ptr z = treeFindLib(tree,data);
    if(!z) return NULL;
    logDebug("Node to remove: %p", z);
    treeRemoveLib(tree, z);
    logDebug("Node %p removed, freeing", z);
    void* ret_data = z->data;
    free(z);
    return ret_data;
}
void treeRebuild(rb_tree_ptr tree, int (*sorting_criteria)(void*, void*)) {
    rb_tree_t tmp_tree;
    tmp_tree = (rb_tree_t){
        .root = global_nil,
        .sorting_criteria = sorting_criteria,
        .data_print = tree->data_print,
        .size = 0};
    
    logInfo("Rebuilding tree %p", tree);

    while(treeSize(tree))
        treeInsertLib(&tmp_tree, treeRemoveLib(tree, tree->root));

    (*tree) = tmp_tree;
}
tree_node_ptr treeFindRecursive(rb_tree_ptr tree, void* x, tree_node_ptr y) {
    if(y == global_nil) return NULL;

    int res = tree->sorting_criteria(x, y->data);

    if (res > 0)
        return treeFindRecursive(tree, x, y->right);
    else if (res < 0)
        return treeFindRecursive(tree, x, y->left);
    
    return y;
}

tree_node_ptr treeFindLib(rb_tree_ptr tree, void* data_ptr) { return treeFindRecursive(tree, data_ptr, tree->root); }
void* treeFind(rb_tree_ptr tree, void* data_ptr) { return treeFindLib(tree, data_ptr)->data; }


void treeLeftRotate(rb_tree_ptr tree, tree_node_ptr x) {
    tree_node_ptr y = x->right;
    x->right = y->left;

    if (y->left != global_nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == global_nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void treeRightRotate(rb_tree_ptr tree, tree_node_ptr x) {
    tree_node_ptr y = x->left;
    x->left = y->right;

    if (y->right != global_nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == global_nil) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void treeTransplant(rb_tree_ptr tree, tree_node_ptr u, tree_node_ptr v){
    if (u->parent == global_nil){
        tree->root = v;
    } else if (u == u->parent->left){
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

tree_node_ptr treeInsertLib(rb_tree_ptr tree, tree_node_ptr z){
    tree_node_ptr y = global_nil;
    tree_node_ptr x = tree->root;
    while(x != global_nil){
        y = x;
        if (tree->sorting_criteria(z->data, x->data) < 0){
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if(y == global_nil){
        tree->root = z;
    } else if (tree->sorting_criteria(z->data, y->data) < 0){
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = global_nil;
    z->right = global_nil;
    z->color = RED;
    tree->size++;
    return(treeInsertFixup(tree, z));
}
tree_node_ptr treeInsertFixup(rb_tree_ptr tree, tree_node_ptr z){
    tree_node_ptr y;
    while (z->parent->color == RED){
        if (z->parent == z->parent->parent->left){
            y = z->parent->parent->right;
            if (y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right){
                    z = z->parent;
                    treeLeftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                treeRightRotate(tree, z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left){
                    z = z->parent;
                    treeRightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                treeLeftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
    logDebug("Node %p inserted into tree %p", z, tree);
    return z;
}

void treeDeleteFixup(rb_tree_ptr tree, tree_node_ptr x){
    while (x != tree->root && x->color == BLACK){
        tree_node_ptr w;
        if (x == x->parent->left){
            w = x->parent->right;
            if (w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                treeLeftRotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK){
                    w->left->color = BLACK;
                    w->color = RED;
                    treeRightRotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                treeLeftRotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            w = x->parent->left;
            if (w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                treeRightRotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK){
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK){
                    w->right->color = BLACK;
                    w->color = RED;
                    treeLeftRotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                treeRightRotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}
void treeDestroyRec(rb_tree_ptr tree, tree_node_ptr node) {
    if(node->left != global_nil) {
        treeDestroyRec(tree, node->left);
    }
    if(node->right != global_nil) {
        treeDestroyRec(tree, node->right);
    }
    free(node);
}
void treeDestroy(rb_tree_ptr tree) {
    if(treeSize(tree)) {
        treeDestroyRec(tree, tree->root);
    }
    logInfo("Removing tree %p", tree);
    free(tree);
    tree_counter--;
    logInfo("Remaining trees: %d", tree_counter);
    if(!tree_counter) {
        free(global_nil);
        logInfo("Common nil removed");
    }
}