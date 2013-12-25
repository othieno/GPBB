/* *** Listing 20.1 ***
 *
 * Draws an ellipse of the specified X and Y axis radii and color,
 * using a fast integer-only & square-root-free approach, and
 * generating the arc for one octant into a buffer, drawing four
 * symmetries from that buffer, then doing the same for the other
 * axis.
 * VGA or EGA.
 * Compiled with Borland C++ 4.02.  Link with L20-2.C.
 * Checked by Jim Mischel 11/30/94
 */
#include <dos.h>

/* Handle differences between Turbo C & MSC. Note that Turbo C accepts
   outp as a synonym for outportb, but not outpw for outport */
#ifdef __TURBOC__
#define outpw  outport
#endif

#define SCREEN_WIDTH_IN_BYTES 80   	/* # of bytes across one scan
                    			   line in mode 12h */
#define SCREEN_SEGMENT        0xA000  	/* mode 12h display memory seg */
#define GC_INDEX              0x3CE	/* Graphics Controller port */
#define SET_RESET_INDEX       0		/* Set/Reset reg index in GC */
#define SET_RESET_ENABLE_INDEX 1    	/* Set/Reset Enable reg index
                                             in GC */
#define BIT_MASK_INDEX        8     	/* Bit Mask reg index in GC */

unsigned char PixList[SCREEN_WIDTH_IN_BYTES*8/2];
                                    /* maximum major axis length is
                                       1/2 screen width, because we're
                                       assuming no clipping is needed */

/* Draws the arc for an octant in which Y is the major axis. (X,Y) is the
   starting point of the arc. HorizontalMoveDirection selects whether the
   arc advances to the left or right horizontally (0=left, 1=right).
   RowOffset contains the offset in bytes from one scan line to the next,
   controlling whether the arc is drawn up or down. Length is the
   vertical length in pixels of the arc, and DrawList is a list
   containing 0 for each point if the next point is vertically aligned,
   and 1 if the next point is 1 pixel diagonally to the left or right */
void DrawVOctant(int X, int Y, int Length, int RowOffset,
   int HorizontalMoveDirection, unsigned char *DrawList)
{
   unsigned char far *ScreenPtr, BitMask;

   /* Point to the byte the initial pixel is in */
#ifdef __TURBOC__
   ScreenPtr = MK_FP(SCREEN_SEGMENT,
      (Y * SCREEN_WIDTH_IN_BYTES) + (X / 8));
#else
   FP_SEG(ScreenPtr) = SCREEN_SEGMENT;
   FP_OFF(ScreenPtr) =(Y * SCREEN_WIDTH_IN_BYTES) + (X / 8);
#endif
   /* Set the initial bit mask */
   BitMask = 0x80 >> (X & 0x07);

   /* Draw all points in DrawList */
   while ( Length-- ) {
      /* Set the bit mask for the pixel */
      outp(GC_INDEX + 1, BitMask);
      /* Draw the pixel. ORed to force read/write to load latches.
         Data written doesn't matter, because set/reset is enabled
         for all planes. Note: don't OR with 0; MSC optimizes that
         statement to no operation */
      *ScreenPtr |= 0xFE;
      /* Now advance to the next pixel based on DrawList */
      if ( *DrawList++ ) {
         /* Advance horizontally to produce a diagonal move. Rotate
            the bit mask, advancing one byte horizontally if the bit
            mask wraps */
         if ( HorizontalMoveDirection == 1 ) {
            /* Move right */
            if ( (BitMask >>= 1) == 0 ) {
               BitMask = 0x80;   /* wrap the mask */
               ScreenPtr++;      /* advance 1 byte to the right */
            }
         } else {
            /* Move left */
            if ( (BitMask <<= 1) == 0 ) {
               BitMask = 0x01;   /* wrap the mask */
               ScreenPtr--;      /* advance 1 byte to the left */
            }
         }
      }
      ScreenPtr += RowOffset; /* advance to the next scan line */
   }
}

/* Draws the arc for an octant in which X is the major axis. (X,Y) is the
   starting point of the arc. HorizontalMoveDirection selects whether the
   arc advances to the left or right horizontally (0=left, 1=right).
   RowOffset contains the offset in bytes from one scan line to the next,
   controlling whether the arc is drawn up or down. Length is the
   horizontal length in pixels of the arc, and DrawList is a list
   containing 0 for each point if the next point is horizontally aligned,
   and 1 if the next point is 1 pixel above or below diagonally */
void DrawHOctant(int X, int Y, int Length, int RowOffset,
   int HorizontalMoveDirection, unsigned char *DrawList)
{
   unsigned char far *ScreenPtr, BitMask;

   /* Point to the byte the initial pixel is in */
#ifdef __TURBOC__
   ScreenPtr = MK_FP(SCREEN_SEGMENT,
      (Y * SCREEN_WIDTH_IN_BYTES) + (X / 8));
#else
   FP_SEG(ScreenPtr) = SCREEN_SEGMENT;
   FP_OFF(ScreenPtr) =(Y * SCREEN_WIDTH_IN_BYTES) + (X / 8);
#endif
   /* Set the initial bit mask */
   BitMask = 0x80 >> (X & 0x07);

   /* Draw all points in DrawList */
   while ( Length-- ) {
      /* Set the bit mask for the pixel */
      outp(GC_INDEX + 1, BitMask);
      /* Draw the pixel (see comments above for details) */
      *ScreenPtr |= 0xFE;
      /* Now advance to the next pixel based on DrawList */
      if ( *DrawList++ ) {
         /* Advance vertically to produce a diagonal move */
         ScreenPtr += RowOffset; /* advance to the next scan line */
      }
      /* Advance horizontally. Rotate the bit mask, advancing one
         byte horizontally if the bit mask wraps */
      if ( HorizontalMoveDirection == 1 ) {
         /* Move right */
         if ( (BitMask >>= 1) == 0 ) {
            BitMask = 0x80;   /* wrap the mask */
            ScreenPtr++;      /* advance 1 byte to the right */
         }
      } else {
         /* Move left */
         if ( (BitMask <<= 1) == 0 ) {
            BitMask = 0x01;   /* wrap the mask */
            ScreenPtr--;      /* advance 1 byte to the left */
         }
      }
   }
}

/* Draws an ellipse of X axis radius A and Y axis radius B in
 * color Color centered at screen coordinate (X,Y). Radii must
 * both be non-zero */
void DrawEllipse(int X, int Y, int A, int B, int Color) {
   int WorkingX, WorkingY;
   long Threshold;
   long ASquared = (long) A * A;
   long BSquared = (long) B * B;
   long XAdjust, YAdjust;
   unsigned char *PixListPtr;

   /* Set drawing color via set/reset */
   outpw(GC_INDEX, (0x0F << 8) | SET_RESET_ENABLE_INDEX);
                                 /* enable set/reset for all planes */
   outpw(GC_INDEX, (Color << 8) | SET_RESET_INDEX);
                                 /* set set/reset (drawing) color */
   outp(GC_INDEX, BIT_MASK_INDEX); /* leave the GC Index reg pointing
                                       to the Bit Mask reg */

   /* Draw the four symmetric arcs for which X advances faster (that is,
      for which X is the major axis) */

   /* Draw the four arcs; set draw parameters for initial point (0,B) */
   /* Calculate all points along an arc of 1/8th of the ellipse and
      store that info in PixList for later drawing */
   PixListPtr = PixList;
   WorkingX = 0;
   XAdjust = 0;
   YAdjust = ASquared * 2 * B;
   Threshold = ASquared / 4 - ASquared * B;

   for (;;) {
      /* Advance the threshold to the value for the next X point
         to be drawn */
      Threshold += XAdjust + BSquared;

      /* If the threshold has passed 0, then the Y coordinate has
         advanced more than halfway to the next pixel and it's time
         to advance the Y coordinate by 1 and set the next threshold
         accordingly */
      if ( Threshold >= 0 ) {
         YAdjust -= ASquared * 2;
         Threshold -= YAdjust;
         *PixListPtr++ = 1;   /* advance along both axes */
      } else {
         *PixListPtr++ = 0;   /* advance only along the X axis */
      }

      /* Advance the X coordinate by 1 */
      XAdjust += BSquared * 2;
      WorkingX++;

      /* Stop if X is no longer the major axis (the arc has passed the
         45-degree point) */
      if ( XAdjust >= YAdjust )
         break;
   }

   /* Now draw each of 4 symmetries of the octant in turn, the
      octants for which X is the major axis. Adjust every other arc
      so that there's no overlap */
   DrawHOctant(X,Y-B,WorkingX,SCREEN_WIDTH_IN_BYTES,0,PixList);
   DrawHOctant(X+1,Y-B+(*PixList),WorkingX-1,SCREEN_WIDTH_IN_BYTES,1,
      PixList+1);
   DrawHOctant(X,Y+B,WorkingX,-SCREEN_WIDTH_IN_BYTES,0,PixList);
   DrawHOctant(X+1,Y+B-(*PixList),WorkingX-1,-SCREEN_WIDTH_IN_BYTES,1,
      PixList+1);

   /* Draw the four symmetric arcs for which X advances faster (that is,
      for which Y is the major axis) */

   /* Draw the four arcs; set draw parameters for initial point (A,0) */
   /* Calculate all points along an arc of 1/8th of the ellipse and
      store that info in PixList for later drawing */
   PixListPtr = PixList;
   WorkingY = 0;
   XAdjust = BSquared * 2 * A;
   YAdjust = 0;
   Threshold = BSquared / 4 - BSquared * A;

   for (;;) {
      /* Advance the threshold to the value for the next Y point
         to be drawn */
      Threshold += YAdjust + ASquared;

      /* If the threshold has passed 0, then the X coordinate has
         advanced more than halfway to the next pixel and it's time
         to advance the X coordinate by 1 and set the next threhold
         accordingly */
      if ( Threshold >= 0 ) {
         XAdjust -= BSquared * 2;
         Threshold = Threshold - XAdjust;
         *PixListPtr++ = 1;   /* advance along both axes */
      } else {
         *PixListPtr++ = 0;   /* advance only along the X axis */
      }

      /* Advance the Y coordinate by 1 */
      YAdjust += ASquared * 2;
      WorkingY++;

      /* Stop if Y is no longer the major axis (the arc has passed the
         45-degree point) */
      if ( YAdjust > XAdjust )
         break;
   }

   /* Now draw each of 4 symmetries of the octant in turn, the
      octants for which Y is the major axis. Adjust every other arc
      so that there's no overlap */
   DrawVOctant(X-A,Y,WorkingY,-SCREEN_WIDTH_IN_BYTES,1,PixList);
   DrawVOctant(X-A+(*PixList),Y+1,WorkingY-1,SCREEN_WIDTH_IN_BYTES,1,
      PixList+1);
   DrawVOctant(X+A,Y,WorkingY,-SCREEN_WIDTH_IN_BYTES,0,PixList);
   DrawVOctant(X+A-(*PixList),Y+1,WorkingY-1,SCREEN_WIDTH_IN_BYTES,0,
      PixList+1);

   /* Reset the Bit Mask register to normal */
   outp(GC_INDEX + 1, 0xFF);
   /* Turn off set/reset enable */
   outpw(GC_INDEX, (0x00 << 8) | SET_RESET_ENABLE_INDEX);
}

