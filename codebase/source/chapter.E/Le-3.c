/* *** Listing 20.3 ***
 *
 * Draws an ellipse of the specified X and Y axis radii and color,
 * using a fast integer-only & square-root-free approach, and
 * generating the arc for one octant into a buffer, drawing four
 * symmetries from that buffer, then doing the same for the other
 * axis. Uses assembly language for inner loops of octant generation
 * & drawing.
 * VGA or EGA.
 * Compiled with Borland C++ 4.02.  Link with L20-2.C and L20-4.ASM.
 * Checked by Jim Mischel 11/30/94.
 * VGA or EGA.
 */
#define ISVGA  0              /* set to 1 to use VGA write mode 3*/
                              /* keep synchronized with Listing 4 */
#include <dos.h>

/* Handle differences between Turbo C & MSC. Note that Turbo C accepts
   outp as a synonym for outportb, but not outpw for outport */
#ifdef __TURBOC__
#define outpw  outport
#endif

#define SCREEN_WIDTH_IN_BYTES 80    /* # of bytes across one scan
                                       line in mode 12h */
#define SCREEN_SEGMENT        0xA000 /* mode 12h display memory seg */
#define GC_INDEX              0x3CE /* Graphics Controller port */
#define SET_RESET_INDEX       0     /* Set/Reset reg index in GC */
#define SET_RESET_ENABLE_INDEX 1    /* Set/Reset Enable reg index
                                       in GC */
#define GC_MODE_INDEX         5  /* Graphics Mode reg index in GC */
#define COLOR_DONT_CARE       7  /* Color Don't Care reg index in GC */
#define BIT_MASK_INDEX        8     /* Bit Mask reg index in GC */

unsigned char PixList[SCREEN_WIDTH_IN_BYTES*8/2];
                                    /* maximum major axis length is
                                       1/2 screen width, because we're
                                       assuming no clipping is needed */

/* Draws an ellipse of X axis radius A and Y axis radius B in
 * color Color centered at screen coordinate (X,Y). Radii must
 * both be non-zero */
void DrawEllipse(int X, int Y, int A, int B, int Color) {
   int Length;
   long Threshold;
   long ASquared = (long) A * A;
   long BSquared = (long) B * B;
   long XAdjust, YAdjust;
   unsigned char *PixListPtr, OriginalGCMode;

   /* Set drawing color via set/reset */
   outpw(GC_INDEX, (0x0F << 8) | SET_RESET_ENABLE_INDEX);
                                 /* enable set/reset for all planes */
   outpw(GC_INDEX, (Color << 8) | SET_RESET_INDEX);
                                 /* set set/reset (drawing) color */
#if ISVGA
   /* Remember original read/write mode & select
      read mode 1/write mode 3, with Color Don't Care
      set to ignore all planes and therefore always return 0xFF */
   outp(GC_INDEX, GC_MODE_INDEX);
   OriginalGCMode = inp(GC_INDEX + 1);
   outp(GC_INDEX+1, OriginalGCMode | 0x0B);
   outpw(GC_INDEX, (0x00 << 8) | COLOR_DONT_CARE);
   outpw(GC_INDEX, (0xFF << 8) | BIT_MASK_INDEX);
#else
   outp(GC_INDEX, BIT_MASK_INDEX); /* leave the GC Index reg pointing
                                       to the Bit Mask reg */
#endif

   /* Draw the four symmetric arcs for which X advances faster (that is,
      for which X is the major axis) */
   /* Generate the draw list for 1 octant */
   Length = GenerateEOctant(PixList, (long) ASquared * 2 * B,
      (long) ASquared / 4 - ASquared * B, ASquared, BSquared);

   /* Now draw each of 4 symmetries of the octant in turn, the
      octants for which X is the major axis. Adjust every other arc
      so that there's no overlap */
   DrawHOctant(X,Y-B,Length,SCREEN_WIDTH_IN_BYTES,0,PixList);
   DrawHOctant(X+1,Y-B+(*PixList),Length-1,SCREEN_WIDTH_IN_BYTES,1,
      PixList+1);
   DrawHOctant(X,Y+B,Length,-SCREEN_WIDTH_IN_BYTES,0,PixList);
   DrawHOctant(X+1,Y+B-(*PixList),Length-1,-SCREEN_WIDTH_IN_BYTES,1,
      PixList+1);

   /* Draw the four symmetric arcs for which Y advances faster (that is,
      for which Y is the major axis) */
   /* Generate the draw list for 1 octant */
   Length = GenerateEOctant(PixList, (long) BSquared * 2 * A,
      (long) BSquared / 4 - BSquared * A, BSquared, ASquared);

   /* Now draw each of 4 symmetries of the octant in turn, the
      octants for which X is the major axis. Adjust every other arc
      so that there's no overlap */
   DrawVOctant(X-A,Y,Length,-SCREEN_WIDTH_IN_BYTES,1,PixList);
   DrawVOctant(X-A+(*PixList),Y+1,Length-1,SCREEN_WIDTH_IN_BYTES,1,
      PixList+1);
   DrawVOctant(X+A,Y,Length,-SCREEN_WIDTH_IN_BYTES,0,PixList);
   DrawVOctant(X+A-(*PixList),Y+1,Length-1,SCREEN_WIDTH_IN_BYTES,0,
      PixList+1);

#if ISVGA
   /* Restore original write mode */
   outpw(GC_INDEX, (OriginalGCMode << 8) | GC_MODE_INDEX);
   /* Restore normal Color Don't Care setting */
   outpw(GC_INDEX, (0x0F << 8) | COLOR_DONT_CARE);
#else
   /* Reset the Bit Mask register to normal */
   outp(GC_INDEX + 1, 0xFF);
#endif
   /* Turn off set/reset enable */
   outpw(GC_INDEX, (0x00 << 8) | SET_RESET_ENABLE_INDEX);
}
