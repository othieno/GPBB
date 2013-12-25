/* Fixed point matrix arithmetic functions
Tested with Borland C++ 4.02 in small model by Jim Mischel 12/16/94.
*/

#include "polygon.h"

/* Matrix multiplies Xform by SourceVec, and stores the result in DestVec. 
Multiplies a 4x4 matrix times a 4x1 matrix; the result is a 4x1 matrix. Cheats 
by assuming the W coord is 1 and bottom row of matrix is 0 0 0 1, and doesn't 
bother to set the W coordinate of the destination */
void XformVec(Xform WorkingXform, Fixedpoint *SourceVec,
   Fixedpoint *DestVec)
{
   int i;

   for (i=0; i<3; i++)
      DestVec[i] = FixedMul(WorkingXform[i][0], SourceVec[0]) +
            FixedMul(WorkingXform[i][1], SourceVec[1]) +
            FixedMul(WorkingXform[i][2], SourceVec[2]) +
            WorkingXform[i][3];   /* no need to multiply by W = 1 */
}

/* Matrix multiplies SourceXform1 by SourceXform2 and stores result in 
 DestXform. Multiplies a 4x4 matrix times a 4x4 matrix; result is a 4x4 matrix.
 Cheats by assuming bottom row of each matrix is 0 0 0 1, and doesn't bother 
 to set the bottom row of the destination */
void ConcatXforms(Xform SourceXform1, Xform SourceXform2,
   Xform DestXform)
{
   int i, j;

   for (i=0; i<3; i++) {
      for (j=0; j<4; j++)
         DestXform[i][j] =
               FixedMul(SourceXform1[i][0], SourceXform2[0][j]) +
               FixedMul(SourceXform1[i][1], SourceXform2[1][j]) +
               FixedMul(SourceXform1[i][2], SourceXform2[2][j]) +
               SourceXform1[i][3];
   }
}

