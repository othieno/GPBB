void WalkBSPTree(NODE *pNode)
{
   if (WallFacingForward(pNode) {
      if (pNode->BackChild) {
         WalkBSPTree(pNode->BackChild);
      }
      Draw(pNode);
      if (pNode->FrontChild) {
         WalkBSPTree(pNode->FrontChild);
      }
   } else {
      if (pNode->FrontChild) {
         WalkBSPTree(pNode->FrontChild);
      }
      Draw(pNode);
      if (pNode->BackChild) {
         WalkBSPTree(pNode->BackChild);
      }
   }
}
