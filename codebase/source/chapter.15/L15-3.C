/* Deletes the node in the specified linked list that follows the
   indicated node. List is headed by a head-of-list pointer; if the
   pointer to the node to delete after points to the head-of-list
   pointer, special handling is performed. */

#include "llist.h"

struct LinkNode *DeleteNodeAfter(struct LinkNode **HeadOfListPtr,
   struct LinkNode *NodeToDeleteAfter)
{
   /* Handle specially if the node to delete after is actually the
      head of the list (delete the first element in the list) */
   if (NodeToDeleteAfter == (struct LinkNode *)HeadOfListPtr) {
		*HeadOfListPtr = (*HeadOfListPtr)->NextNode;
   } else {
      NodeToDeleteAfter->NextNode =
            NodeToDeleteAfter->NextNode->NextNode;
   }
   return(NodeToDeleteAfter);
}

