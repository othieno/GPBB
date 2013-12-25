/* *** Listing 17.1 ***
 *
 * Draws a circle of the specified radius and color.
 * Will work on VGA or EGA in 16-color mode.
 * Compiled with Borland C++ 4.02 and linked with L17-2.C in small model.
 * Checked by Jim Mischel 11/30/94
 */

#include <math.h>
#include <dos.h>

/* Handle differences between Turbo C & MSC. Note that Turbo C accepts
   outp as a synonym for outportb, but not outpw for outport */
#ifdef __TURBOC__
#define outpw	outport
#endif

#define SCREEN_WIDTH_IN_BYTES	80	/* # of bytes across one scan
					   line in mode 12h */
#define SCREEN_SEGMENT 0xA000 		/* mode 12h display memory seg */
#define GC_INDEX	0x3CE			/* Graphics Controller port */
#define SET_RESET_INDEX 0		/* Set/Reset reg index in GC */
#define SET_RESET_ENABLE_INDEX 1		/* Set/Reset enable reg index in GC */
#define BIT_MASK_INDEX 8			/* Bit Mask reg index in GC */

/* Draws a pixel at screen coordinate (X,Y) */
void DrawDot(int X, int Y) {
	unsigned char far *ScreenPtr;

   /* Point to the byte the pixel is in */
#ifdef __TURBOC__
   ScreenPtr = MK_FP(SCREEN_SEGMENT,
   (Y * SCREEN_WIDTH_IN_BYTES) + (X / 8));
#else
   FP_SEG(ScreenPtr) = SCREEN_SEGMENT;
   FP_OFF(ScreenPtr) =(Y * SCREEN_WIDTH_IN_BYTES) + (X / 8);
#endif

/* Set the bit mask within the byte for the pixel */
   outp(GC_INDEX + 1, 0x80 >> (X & 0x07));

/* Draw the pixel. ORed to force read/write to load latches.
   Data written doesn't matter, because set/reset is enabled
   for all planes. Note: don't OR with 0; MSC optimizes that
   statement to no operation */
   *ScreenPtr |= 0xFE;
}

/* Draws a circle of radius Radius in color Color centered at
 * screen coordinate (X,Y) */
void DrawCircle(int X, int Y, int Radius, int Color) {
	int MajorAxis, MinorAxis;
   double RadiusSquared = (double) Radius * Radius;

   /* Set drawing color via set/reset */
   outpw(GC_INDEX, (0x0F << 8) | SET_RESET_ENABLE_INDEX);
   /* enable set/reset for all planes */
   outpw(GC_INDEX, (Color << 8) | SET_RESET_INDEX);
   /* set set/reset (drawing) color */
   outp(GC_INDEX, BIT_MASK_INDEX);
   /* leave the GC pointing to Bit Mask reg */

   /* Set up to draw the circle by setting the initial point to one
   end of the 1/8th of a circle arc we'll draw */
   MajorAxis = 0;
   MinorAxis = Radius;

   /* Draw all points along an arc of 1/8th of the circle, drawing
   all 8 symmetries at the same time */
   do {
   /* Draw all 8 symmetries of current point */
      DrawDot(X+MajorAxis, Y-MinorAxis);
      DrawDot(X-MajorAxis, Y-MinorAxis);
      DrawDot(X+MajorAxis, Y+MinorAxis);
      DrawDot(X-MajorAxis, Y+MinorAxis);
      DrawDot(X+MinorAxis, Y-MajorAxis);
      DrawDot(X-MinorAxis, Y-MajorAxis);
      DrawDot(X+MinorAxis, Y+MajorAxis);
      DrawDot(X-MinorAxis, Y+MajorAxis);
      MajorAxis++;		/* advance one pixel along major axis */
      MinorAxis =
	sqrt(RadiusSquared - ((double) MajorAxis * MajorAxis)) + 0.5;
	/* calculate corresponding point along minor axis */
  } while ( MajorAxis <= MinorAxis );

	/* Reset the Bit Mask register to normal */
	outp(GC_INDEX + 1, 0xFF);

	/* Turn off set/reset enable */
	outpw(GC_INDEX, (0x00 << 8) | SET_RESET_ENABLE_INDEX);
}
