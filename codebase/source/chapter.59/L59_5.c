// Sample program to exercise and time the performance of
// implementations of WalkTree().
// Tested with 32-bit Visual C++ 1.10.

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "tree.h"

long VisitCount = 0;

void main(void);
void BuildTree(NODE *pNode, int RemainingDepth);
extern void WalkTree(NODE *pRootNode);

void main()
{
   NODE RootNode;
   int i;
   long StartTime;

   // Build a sample tree

   BuildTree(&RootNode, 14);

   // Walk the tree 1000 times and see how long it takes

   StartTime = time(NULL);

   for (i=0; i<1000; i++)
   {
      WalkTree(&RootNode);
   }

   printf("Seconds elapsed: %ld\n",
           time(NULL) - StartTime);
   getch();
}

//
// Function to add right and left subtrees of the
// specified depth off the passed-in node.
//
void BuildTree(NODE *pNode, int RemainingDepth)
{
   if (RemainingDepth == 0)
   {
      pNode->pLeftChild = NULL;
      pNode->pRightChild = NULL;
   }
   else
   {
      pNode->pLeftChild = malloc(sizeof(NODE));
      if (pNode->pLeftChild == NULL)
      {
         printf("Out of memory\n");
         exit(1);
      }
      pNode->pRightChild = malloc(sizeof(NODE));
      if (pNode->pRightChild == NULL)
      {
         printf("Out of memory\n");
         exit(1);
      }

      BuildTree(pNode->pLeftChild, RemainingDepth - 1);
      BuildTree(pNode->pRightChild, RemainingDepth - 1);
   }
}

//
// Node-visiting function so WalkTree() has something to
// call.
//
void Visit(NODE *pNode)
{
   VisitCount++;
}
