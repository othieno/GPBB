/* Draws all pixels in the list of horizontal lines passed in, in Hicolor 
(32K color) mode on an ET4000-based SuperVGA. Uses a slow pixel-by-pixel 
approach. 
  Tested with Borland C++ 4.02 in small model by Jim Mischel 12/16/94. */

#include <dos.h>
#include "polygon.h"
#define SCREEN_SEGMENT     0xA000
#define GC_SEGMENT_SELECT  0x3CD

void DrawPixel(int, int, int);
extern int BitmapWidthInBytes; /* # of pixels per line */

void DrawHCLineList(struct HLineList * HLineListPtr,
      int Color)
{
   struct HLine *HLinePtr;
   int Y, X;

   /* Point to XStart/XEnd descriptor for the first (top) horizontal line */
   HLinePtr = HLineListPtr->HLinePtr;
   /* Draw each horizontal line in turn, starting with the top one and
      advancing one line each time */
   for (Y = HLineListPtr->YStart; Y < (HLineListPtr->YStart +
         HLineListPtr->Length); Y++, HLinePtr++) {
      /* Draw each pixel in the current horizontal line in turn,
         starting with the leftmost one */
      for (X = HLinePtr->XStart; X <= HLinePtr->XEnd; X++)
         DrawPixel(X, Y, Color);
   }
}

/* Draws the pixel at (X, Y) in color Color in Hicolor mode on an
   ET4000-based SuperVGA */
void DrawPixel(int X, int Y, int Color) {
   unsigned int far *ScreenPtr, Bank;
   unsigned long BitmapAddress;

   /* Full bitmap address of pixel, as measured from address 0 to
      address 0xFFFFF. (X << 1) because pixels are 2 bytes in size */
   BitmapAddress = (unsigned long) Y * BitmapWidthInBytes + (X << 1);
   /* Map in the proper bank. Bank # is upper word of bitmap addr */
   Bank = *(((unsigned int *)&BitmapAddress) + 1);
   /* Upper nibble is read bank #, lower nibble is write bank # */
   outp(GC_SEGMENT_SELECT, (Bank << 4) | Bank);
   /* Draw into the bank */
   FP_SEG(ScreenPtr) = SCREEN_SEGMENT;
   FP_OFF(ScreenPtr) = *((unsigned int *)&BitmapAddress);
   *ScreenPtr = (unsigned int)Color;
}

