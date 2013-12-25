/* Returns 1 if polygon described by passed-in vertex list is monotone with
respect to a vertical line, 0 otherwise. Doesn't matter if polygon is simple 
(non-self-intersecting) or not.
  Tested with Borland C++ 4.02 in small model by Jim Mischel 12/16/94.
  Requires: L24-2.C L24-3.ASM L24-4.C L22-4.ASM L23-4.C L23-5.ASM
  Compile command line:
    bcc -ms l24-1.c l24-2.c l24-3.asm l24-4.c l22-4.asm l23-4.c l23-5.asm
*/

#include "polygon.h"

#define SIGNUM(a) ((a>0)?1:((a<0)?-1:0))

int PolygonIsMonotoneVertical(struct PointListHeader * VertexList)
{
   int i, Length, DeltaYSign, PreviousDeltaYSign;
   int NumYReversals = 0;
   struct Point *VertexPtr = VertexList->PointPtr;

   /* Three or fewer points can't make a non-vertical-monotone polygon */
   if ((Length=VertexList->Length) < 4) return(1);

   /* Scan to the first non-horizontal edge */
   PreviousDeltaYSign = SIGNUM(VertexPtr[Length-1].Y - VertexPtr[0].Y);
   i = 0;
   while ((PreviousDeltaYSign == 0) && (i < (Length-1))) {
      PreviousDeltaYSign = SIGNUM(VertexPtr[i].Y - VertexPtr[i+1].Y);
      i++;
   }

   if (i == (Length-1)) return(1);  /* polygon is a flat line */

   /* Now count Y reversals. Might miss one reversal, at the last vertex, but 
      because reversal counts must be even, being off by one isn't a problem */
   do {
      if ((DeltaYSign = SIGNUM(VertexPtr[i].Y - VertexPtr[i+1].Y))
            != 0) {
         if (DeltaYSign != PreviousDeltaYSign) {
            /* Switched Y direction; not vertical-monotone if
               reversed Y direction as many as three times */
            if (++NumYReversals > 2) return(0);
            PreviousDeltaYSign = DeltaYSign;
         }
      }
   } while (i++ < (Length-1));
   return(1);  /* it's a vertical-monotone polygon */
}

