/* *** Listing 19.1 ***
 *
 * Draws an ellipse of the specified X and Y axis radii and color,
 * using floating-point calculations.
 * VGA or EGA.
 * Compile with Borland C++ 4.02 and link with L19-2.C or L19-3.C.
 * Checked by Jim Mischel 11/30/94.
 */

#include <math.h>
#include <dos.h>

/* Turbo C accepts outp for outportb, but not outpw for outport */
#ifdef __TURBOC__
#define outpw  outport
#endif

#define SCREEN_WIDTH_IN_BYTES 80    /* # of bytes across one scan
                                       line in modes 10h and 12h */
#define SCREEN_SEGMENT        0xA000 /* mode 10h/12h display memory seg */
#define GC_INDEX              0x3CE /* Graphics Controller Index port */
#define SET_RESET_INDEX       0     /* Set/Reset reg index in GC */
#define SET_RESET_ENABLE_INDEX 1    /* Set/Reset Enable reg index in GC */
#define BIT_MASK_INDEX        8     /* Bit Mask reg index in GC */

/* Draws a pixel at screen coordinate (X,Y) */
void DrawDot(int X, int Y) {
   unsigned char far *ScreenPtr;

   /* Point to the byte the pixel is in */
#ifdef __TURBOC__
   ScreenPtr = MK_FP(SCREEN_SEGMENT, (Y*SCREEN_WIDTH_IN_BYTES) + (X/8));
#else
   FP_SEG(ScreenPtr) = SCREEN_SEGMENT;
   FP_OFF(ScreenPtr) = (Y * SCREEN_WIDTH_IN_BYTES) + (X / 8);
#endif

   /* Set the bit mask within the byte for the pixel */
   outp(GC_INDEX + 1, 0x80 >> (X & 0x07));

   /* Draw the pixel. ORed to force read/write to load latches.
      Data written doesn't matter, because set/reset is enabled
      for all planes. Note: don't OR with 0; MSC optimizes that
      statement to no operation */
   *ScreenPtr |= 0xFE;
}

/* Draws an ellipse of X axis radius A and Y axis radius B in
 * color Color centered at screen coordinate (X,Y). Radii must
 * both be non-zero */
void DrawEllipse(int X, int Y, int A, int B, int Color) {
   int WorkingX, WorkingY;
   double ASquared = (double) A * A;
   double BSquared = (double) B * B;
   double Temp;

   /* Set drawing color via set/reset */
   outpw(GC_INDEX, (0x0F << 8) | SET_RESET_ENABLE_INDEX);
                                 /* enable set/reset for all planes */
   outpw(GC_INDEX, (Color << 8) | SET_RESET_INDEX);
                                 /* set set/reset (drawing) color */
   outp(GC_INDEX, BIT_MASK_INDEX); /* leave the GC Index reg pointing
                                       to the Bit Mask reg */

   /* Draw the four symmetric arcs for which X advances faster (that is,
      for which X is the major axis) */
   /* Draw the initial top & bottom points */
   DrawDot(X, Y+B);
   DrawDot(X, Y-B);

   /* Draw the four arcs */
   for (WorkingX = 0; ; ) {
      /* Advance one pixel along the X axis */
      WorkingX++;

      /* Calculate the corresponding point along the Y axis. Guard
         against floating-point roundoff making the intermediate term
         less than 0 */
      Temp = BSquared - (BSquared *
         (double)WorkingX * (double)WorkingX / ASquared);
      if ( Temp >= 0 ) {
         WorkingY = sqrt(Temp) + 0.5;
      } else {
         WorkingY = 0;
      }

      /* Stop if X is no longer the major axis (the arc has passed the
         45-degree point) */
      if (((double)WorkingY/BSquared) <= ((double)WorkingX/ASquared))
         break;

      /* Draw the 4 symmetries of the current point */
      DrawDot(X+WorkingX, Y-WorkingY);
      DrawDot(X-WorkingX, Y-WorkingY);
      DrawDot(X+WorkingX, Y+WorkingY);
      DrawDot(X-WorkingX, Y+WorkingY);
   }

   /* Draw the four symmetric arcs for which Y advances faster (that is,
      for which Y is the major axis) */
   /* Draw the initial left & right points */
   DrawDot(X+A, Y);
   DrawDot(X-A, Y);

   /* Draw the four arcs */
   for (WorkingY = 0; ; ) {
      /* Advance one pixel along the Y axis */
      WorkingY++;

      /* Calculate the corresponding point along the X axis. Guard
         against floating-point roundoff making the intermediate term
         less than 0 */
      Temp = ASquared - (ASquared *
         (double)WorkingY * (double)WorkingY / BSquared);
      if ( Temp >= 0 ) {
         WorkingX = sqrt(Temp) + 0.5;
      } else {
         WorkingX = 0;           /* floating-point roundoff */
      }

      /* Stop if Y is no longer the major axis (the arc has passed the
         45-degree point) */
      if (((double)WorkingX/ASquared) < ((double)WorkingY/BSquared))
         break;

      /* Draw the 4 symmetries of the current point */
      DrawDot(X+WorkingX, Y-WorkingY);
      DrawDot(X-WorkingX, Y-WorkingY);
      DrawDot(X+WorkingX, Y+WorkingY);
      DrawDot(X-WorkingX, Y+WorkingY);
   }

   /* Reset the Bit Mask register to normal */
   outp(GC_INDEX + 1, 0xFF);

   /* Turn off set/reset enable */
   outpw(GC_INDEX, (0x00 << 8) | SET_RESET_ENABLE_INDEX);
}

