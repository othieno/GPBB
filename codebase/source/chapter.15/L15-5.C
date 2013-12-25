/* Finds the first node in a value-sorted linked list that
   has a Value field greater than or equal to a key value, and
   returns a pointer to the node preceding that node (to facilitate
   insertion and deletion), or a NULL pointer if no such value was
   found. Assumes the list is terminated with a sentinel tail node
   containing the largest possible Value field setting and pointing
   to itself as the next node. */

#include <stdio.h>
#include "llist.h"

struct LinkNode *FindNodeBeforeValueNotLess(
   struct LinkNode *HeadOfListNode, int SearchValue)
{
   struct LinkNode *NodePtr = HeadOfListNode;

   while (NodePtr->NextNode->Value < SearchValue)
      NodePtr = NodePtr->NextNode;

   if (NodePtr->NextNode->NextNode == NodePtr->NextNode)
      return(NULL);     /* we found the sentinel; failed search */
   else
      return(NodePtr);  /* success; return pointer to node preceding
                           node that was >= */
}

