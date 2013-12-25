// Function to inorder walk a tree, using code recursion.
// Tested with 32-bit Visual C++ 1.10.

#include <stdlib.h>
#include "tree.h"

extern void Visit(NODE *pNode);

void WalkTree(NODE *pNode)
{
   // Make sure the tree isn't empty
   if (pNode != NULL)
   {
      // Traverse the left subtree, if there is one
      if (pNode->pLeftChild != NULL)
      {
         WalkTree(pNode->pLeftChild);
      }

      // Visit this node
      Visit(pNode);

      // Traverse the right subtree, if there is one
      if (pNode->pRightChild != NULL)
      {
         WalkTree(pNode->pRightChild);
      }
   }
}
