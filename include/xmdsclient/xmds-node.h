#ifndef XMDS_NODE_H
#define XMDS_NODE_H
#include <stdint.h>

typedef enum _xmdsNodeType {
    NODE_FILES_ENTRY = 0,
    NODE_REGISTER_DISPLAY_ENTRY,
    NODE_REGISTER_DISPLAY_DETAIL,
    NODE_SCHEDULE_ENTRY,
    NODE_SCHEDULE_LAYOUT,
    NODE_SCHEDULE_DEPS,
    NODE_LOGXML_ENTRY,
    NODE_STATXML_ENTRY,
    NODE_MEDIA_INVENTORY_ENTRY
} xmdsNodeType;

typedef struct _xmdsNode {
    void *data;

    struct _xmdsNode *first;
    struct _xmdsNode *next;
    struct _xmdsNode *prev;
    struct _xmdsNode *last;

    enum _xmdsNodeType _type;
    uint32_t _id;
    uint32_t _state;
} xmdsNode;

/**
 * @brief create an new node.
 * @return pointer to the newly created node or NULL if failed
 */
xmdsNode *xmdsNodeNew(xmdsNodeType type);

/**
 * @brief append an initialized child node to the end of nodes.
 * @param parent the existing node which at least contains one node
 * @param child node to be appended, created by node_new()
 * @return the new parent length/size after appending the child, or 0 if failed.
 * @see node_new()
 */
int xmdsNodeAppend(xmdsNode **parent, xmdsNode *child);


/**
 * @brief get the length/size of a given node.
 * @param node
 * @return the node length/size
 */
int xmdsNodeLength(xmdsNode *node);

/**
 * @brief get a node from list
 * @param node
 * @param index
 * @return the node at index or NULL if failed.
 */
xmdsNode *xmdsNodeGet(xmdsNode *node, int index);

/**
 * @brief delete the last child of a given node.
 * @param node
 * @return the node length after deletion
 */
int xmdsNodeFreeLast(xmdsNode **node);

int isXmdsNodeType(xmdsNode *node, xmdsNodeType type);

#endif /* XMDS_NODE_H */

