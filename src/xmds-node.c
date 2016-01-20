#include <stdlib.h>
#include <stdio.h>
#include "xmds-node.h"

/**
 * @brief create an new node.
 * @return pointer to the newly created node or NULL if failed
 */
xmdsNode *xmdsNodeNew(xmdsNodeType type) {
    xmdsNode *node;
    static uint32_t counter;

    node = malloc(sizeof(xmdsNode));

    if(node == NULL) {
        fprintf(stderr, "ERROR foonode_new() cannot allocate memory.\n");
        return NULL;
    }

    node->data = NULL;
    node->prev = NULL;
    node->next = NULL;
    node->first = node;
    node->last = node;
    node->_type = type;
    node->_state = 0;
    node->_id = counter++;

    return node;
}

/**
 * @brief append an initialized child node to the end of nodes.
 * @param parent the existing node which at least contains one node
 * @param child node to be appended, created by node_new()
 * @return the new parent length/size after appending the child, or 0 if failed.
 * @see node_new()
 */
int xmdsNodeAppend(xmdsNode **parent, xmdsNode *child) {
    xmdsNode *iter = NULL;

    /* not a node i know */
    if(parent == NULL || child == NULL) {
        fprintf(stderr, "ERROR fonode_append() either parent or child is NULL.\n");
        return 0;
    }

    /* cannot append different type of node */
    if((*parent)->_type != child->_type) {
        fprintf(stderr, "ERROR fonode_append() cannot append different type of node. parent type: %d, child type: %d\n",
                (*parent)->_type, child->_type);
        return 0;
    }

    (*parent)->last->next = child;

    child->first = (*parent)->first;
    child->last = child;
    child->next = NULL;
    child->prev = (*parent)->last;

    iter = (*parent)->first;
    while(iter) {
        iter->last = child;
        iter = iter->next;
    }

    return xmdsNodeLength(*parent);
}

/**
 * @brief move node position to index
 * @param nodes
 * @param index
 */
/*
void node_at(fooNode **nodes, int index) {
    int i = 0;
    *nodes = (*nodes)->first;
    while((*nodes)->next && i != index) {
        *nodes = (*nodes)->next;
        i++;
    }
}
*/
/**
 * @brief get the length/size of a given node.
 * @param node
 * @return the node length/size
 */
int xmdsNodeLength(xmdsNode *node) {
    int length = 0;
    xmdsNode *iter = NULL;

    if(node == NULL) {
        return 0;
    }

    iter = node->first;
    while(iter) {
        length++;
        iter = iter->next;
    }

    return length;
}


xmdsNode *xmdsNodeGet(xmdsNode *node, int index) {
    xmdsNode *iter;
    int i = 0;

    i = index > -1 ? i : 0;

    iter = node->first;
    while(iter) {
        if(i == index) {
            return iter;
        }
        iter = iter->next;
        i++;
    }

    return NULL;
}
/**
 * @brief delete the last child of a given node.
 * @param node
 * @return the node length after deletion
 */
int xmdsNodeFreeLast(xmdsNode **node) {
    xmdsNode *iter = NULL;
    xmdsNode *delme = NULL;
    xmdsNode *last = NULL;

    /* not a node i know */
    if(!node || !(*node)) {
        return -1;
    }

    /* there's only one node, alone.. */
    if((*node)->first == (*node) && (*node)->last == (*node)) {

        free(*node);
        *node = NULL;
        return 0;
    }

    /* the node to delete is current node index? move index to prev */
    if((*node)->last == *node) {
        *node = (*node)->prev;
    }

    /* (*node)->last maybe previously freed */
    if((*node)->last) {
        delme = (*node)->last;
        last = delme->prev;

        last->last = last;
        last->next = NULL;

        iter = (*node)->first;
        while(iter && iter != delme) {
            iter->last = last;
            iter = iter->next;
        }

        free(delme);
        delme = NULL;

    }

    return xmdsNodeLength(*node);
}

/**
 * @brief completely delete a node.
 * @param node
 * @return the length of node after deletion.
 */
/*
int foonode_delete_all(fooNode **node) {
    while(*node) {
        foonode_delete_last(node);
    }

    return foonode_length(*node);
}
*/
int isXmdsNodeType(xmdsNode *node, xmdsNodeType type) {
    return node->_type == type;
}
