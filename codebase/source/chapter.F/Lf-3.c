/* Draws pixels from the list of horizontal lines passed in; drawing
   takes place only for scan lines between ScanBandStart and
   ScanBandEnd, inclusive; drawing goes to ScanLineBuffer, with
   the scan line at ScanBandStart mapping to the first scan line in
   ScanLineBuffer. Intended for use in unweighted antialiasing,
   whereby a polygon is scanned out into a buffer at a multiple of the
   screen's resolution, and then the scanned-out info in the buffer is
   grouped into megapixels that are mapped to the closest
   approximation the screen supports and drawn. 
   Tested with Borland 4.02 in small model by Jim Mischel 12/16/94.
*/
#include <string.h>
#include <dos.h>
#include "polygon.h"

extern unsigned char *ScanLineBuffer;  /* drawing goes here */
extern int ScanBandStart, ScanBandEnd; /* limits of band to draw */
extern int ScanBandWidth;  /* # of pixels across scan band */
   
void DrawBandedList(struct HLineList * HLineListPtr, int Color)
{
   struct HLine *HLinePtr;
   int Length, Width, YStart = HLineListPtr->YStart;
   unsigned char *BufferPtr;

   /* Done if fully off the bottom or top of the band */
   if (YStart > ScanBandEnd) return;
   Length = HLineListPtr->Length;
   if ((YStart + Length) <= ScanBandStart) return;

   /* Point to the XStart/XEnd descriptor for the first (top)
      horizontal line */
   HLinePtr = HLineListPtr->HLinePtr;

   /* Confine drawing to the specified band */
   if (YStart < ScanBandStart) {
      /* Skip ahead to the start of the band */
      Length -= ScanBandStart - YStart;
      HLinePtr += ScanBandStart - YStart;
      YStart = ScanBandStart;
   }
   if (Length > (ScanBandEnd - YStart + 1))
      Length = ScanBandEnd - YStart + 1;

   /* Point to the start of the first scan line on which to draw */
   BufferPtr = ScanLineBuffer + (YStart - ScanBandStart) *
         ScanBandWidth;

   /* Draw each horizontal line within the band in turn, starting with
      the top one and advancing one line each time */
   while (Length-- > 0) {
      /* Draw the whole horizontal line if it has a positive width */
      if ((Width = HLinePtr->XEnd - HLinePtr->XStart + 1) > 0)
         memset(BufferPtr + HLinePtr->XStart, Color, Width);
      HLinePtr++;                /* point to next scan line X info */
      BufferPtr += ScanBandWidth; /* point to next scan line start */
   }
}

