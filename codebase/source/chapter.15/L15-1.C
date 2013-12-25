/* Deletes the node in a linked list that follows the indicated node.
   Assumes list is headed by a dummy node, so no special testing for
   the head-of-list pointer is required.  Returns the same pointer
   that was passed in. */

#include "llist.h"

struct LinkNode *DeleteNodeAfter(struct LinkNode *NodeToDeleteAfter)
{
   NodeToDeleteAfter->NextNode =
         NodeToDeleteAfter->NextNode->NextNode;
   return(NodeToDeleteAfter);
}

