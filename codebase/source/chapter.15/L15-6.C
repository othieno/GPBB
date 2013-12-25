/*
Suite of functions for maintaining a linked list sorted by
ascending order of the Value field. The list is circular; that is,
it has a dummy node as both the head and the tail of the list.
The dummy node is a sentinel, containing the largest possible
Value field setting. Tested with Borland C++ in C mode. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "llist.h"

/* Initializes an empty linked list of LinkNode structures,
   consisting of a single head/tail/sentinel node, and returns a
   pointer to the list. Returns NULL for failure. */
struct LinkNode *InitLinkedList()
{
   struct LinkNode *Sentinel;

   if ((Sentinel = malloc(sizeof(struct LinkNode))) == NULL)
      return(NULL);

   Sentinel->NextNode = Sentinel;
   Sentinel->Value = SENTINEL;
   strcpy(Sentinel->Text, "*** sentinel ***");
   return(Sentinel);
}

/* Finds the first node in a value-sorted linked list with a value
   field equal to a key value, and returns a pointer to the node
   preceding that node (to facilitate insertion and deletion), or a
   NULL pointer if no such value was found. Assumes the list is
   terminated with a sentinel node containing the largest possible
   value. */
struct LinkNode *FindNodeBeforeValue(struct LinkNode *HeadOfListNode,
   int SearchValue)
{
   struct LinkNode *NodePtr = HeadOfListNode;

   while (NodePtr->NextNode->Value < SearchValue)
      NodePtr = NodePtr->NextNode;

   if (NodePtr->NextNode->Value == SearchValue) {
      /* Found the search value; success unless we found the
         sentinel (can happen only if SearchValue == SENTINEL) */
      if (NodePtr->NextNode == HeadOfListNode) {
         return(NULL);     /* failure; we found the sentinel */
      } else {
         return(NodePtr);  /* success; return pointer to node
                              preceding the node that was equal */
      }
   } else {
      /* No match; return failure status */
      return(NULL);
   }
}

/* Inserts the specified node into a value-sorted linked list, such
   that value-sorting is maintained. Returns a pointer to the node
   after which the new node is inserted. */
struct LinkNode *InsertNodeSorted(struct LinkNode *HeadOfListNode,
   struct LinkNode *NodeToInsert)
{
   struct LinkNode *NodePtr = HeadOfListNode;
   int SearchValue = NodeToInsert->Value;

   while (NodePtr->NextNode->Value < SearchValue)
      NodePtr = NodePtr->NextNode;

   NodeToInsert->NextNode = NodePtr->NextNode;
   NodePtr->NextNode = NodeToInsert;
   return(NodePtr);
}

