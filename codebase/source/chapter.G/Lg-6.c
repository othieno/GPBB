/* Draws pixels from the list of horizontal lines passed in, to a 32-bpp 
buffer; drawing takes place only for scan lines between ScanBandStart and 
ScanBandEnd, inclusive; drawing goes to ScanLineBuffer, with the scan line at 
ScanBandStart mapping to the first scan line in ScanLineBuffer. Note that 
Color here points to an RGB structure that maps directly to the buffer's pixel 
format, rather than containing a 16-bit integer.
  Tested with Borland C++ 4.02 in small model by Jim Mischel 12/16/94.
*/
#include "polygon.h"

extern struct RGB *ScanLineBuffer;  /* drawing goes here */
extern int ScanBandStart, ScanBandEnd; /* limits of band to draw */
extern int ScanBandWidth;  /* # of subpixels across scan band */
   
void DrawBandedList(struct HLineList * HLineListPtr,
   struct RGB *Color)
{
   struct HLine *HLinePtr;
   int Length, Width, YStart = HLineListPtr->YStart, i;
   struct RGB *BufferPtr, *WorkingBufferPtr;

   /* Done if fully off the bottom or top of the band */
   if (YStart > ScanBandEnd) return;
   Length = HLineListPtr->Length;
   if ((YStart + Length) <= ScanBandStart) return;

   /* Point to XStart/XEnd descriptor for the first (top) horizontal line */
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
   BufferPtr = ScanLineBuffer + (YStart-ScanBandStart)*ScanBandWidth;

   /* Draw each horizontal line within the band in turn, starting with
      the top one and advancing one line each time */
   while (Length-- > 0) {
      /* Fill whole horiz line with Color if it has positive width */
      if ((Width = HLinePtr->XEnd - HLinePtr->XStart + 1) > 0) {
         WorkingBufferPtr = BufferPtr + HLinePtr->XStart;
         for (i = 0; i < Width; i++) *WorkingBufferPtr++ = *Color;
      }
      HLinePtr++;                /* point to next scan line X info */
      BufferPtr += ScanBandWidth; /* point to start of next line */
   }
}

