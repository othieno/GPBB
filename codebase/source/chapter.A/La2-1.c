/*
 * Illustrates color paging by color-animating a series of
 * concentric circles to produce the illusion of motion.
 * Runs on the VGA only, because color paging isn't available on
 * the EGA.
 *
 * Compiled with Borland C++ 4.02, with the command line:
 *   bcc l12-1.c
 *
 * Checked by Jim Mischel 11/21/94
 */

#include <dos.h>

#define USE_BIOS  0  /* set to 1 to use BIOS functions to perform
                        color paging, 0 to program color paging
                        registers directly */

/* Handle differences between Turbo C & MSC. Note that Turbo C accepts
   outp as a synonym for outportb, but not outpw for outport */

#define SCREEN_WIDTH_IN_BYTES 80    /* # of bytes across one scan
                                       line in mode 12h */
#define SCREEN_SEGMENT        0xA000 /* mode 12h display memory seg */
#define GC_INDEX              0x3CE /* Graphics Controller index */
#define SET_RESET_INDEX       0     /* Set/Reset reg index in GC */
#define SET_RESET_ENABLE_INDEX 1    /* Set/Reset Enable reg index
                                       in GC */
#define BIT_MASK_INDEX        8     /* Bit Mask reg index in GC */
#define INPUT_STATUS_1        0x3DA /* Input Status 1 port */
#define AC_INDEX              0x3C0 /* Attribute Controller index */
#define AC_DATA_W             0x3C0 /* Attribute Controller data
                                       register for writes */
#define AC_DATA_R             0x3C1 /* Attribute Controller data
                                       register for reads */
#define AC_MODE_INDEX         0x30  /* AC Mode reg index, with bit 6
                                       set to avoid blanking screen */
#define AC_COLOR_SELECT_INDEX 0x34  /* Color Select reg index, with
                                       bit 6 set to avoid blanking
                                       screen */

void main();
void DrawDot(int X, int Y);
void DrawCircle(int X, int Y, int Radius, int Color);

/* Array used to load the DAC. Organized as 256 RGB triplets */
static unsigned char DACSettings[256*3];

/* Array used to load the palette RAM to a pass-through state.
   The 17th entry sets the border color to 0 */
static unsigned char PaletteRAMSettings[] = {
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

void main() {
   int Radius, Color, Page, Element, i;
   union REGS Regs;
   struct SREGS Sregs;
   unsigned char GreenComponent;

   /* Select VGA's hi-res 640x480 graphics mode, mode 12h */
   Regs.x.ax = 0x0012;
   int86(0x10, &Regs, &Regs);

   /* Draw concentric circles */
   for ( Radius = 10, Color = 1; Radius < 240; Radius += 2 ) {
      DrawCircle(640/2, 480/2, Radius, Color);
      if (++Color >= 16)
         Color = 1;                 /* skip color 0 */
   }

   /* Load the upper 240 DAC locations (15 pages) with one-position
      rotations of a series of increasingly green colors. Because
      page 0 is being displayed, the screen remains unchanged while
      the other 15 color pages are being loaded */

   /* First, fill DACSettings with the desired green settings
      (because it's a static array, all locations are initialized to
      zero, so we don't need to initialize the red or green color
      components, which we want to be zero) */
   GreenComponent = 8;
   for ( Page = 1; Page <= 15; Page++ ) {
      GreenComponent = Page * 4;
      for ( Element = 1; Element <= 15; Element++ ) {
         DACSettings[Page*16*3 + Element*3 + 1] = GreenComponent;
         if ( (GreenComponent += 4) >= 64 )
            GreenComponent = 4;
      }
   }
      
   /* Now call the BIOS to load the upper 240 DAC locations */
   Regs.h.ah = 0x10;
   Regs.h.al = 0x12;
   Regs.x.bx = 16;
   Regs.x.cx = 240;
   Regs.x.dx = (unsigned int)(DACSettings + 16*3);
   segread(&Sregs);
   Sregs.es = Sregs.ds;       /* point ES:DX to DACSettings */
   int86x(0x10, &Regs, &Regs, &Sregs);

   /* Put the palette RAM in a pass-through state and set the Overscan
      register (border color) to 0. We've saved this for last because
      it changes the colors being displayed */
   Regs.h.ah = 0x10;
   Regs.h.al = 2;
   Regs.x.dx = (unsigned int)PaletteRAMSettings;
   segread(&Sregs);
   Sregs.es = Sregs.ds;       /* point ES:DX to PaletteRAMSettings */
   int86x(0x10, &Regs, &Regs, &Sregs);

   /* Enable 16-pages-of-16-colors paging */
#if USE_BIOS
   Regs.h.ah = 0x10;
   Regs.h.al = 0x13;
   Regs.h.bl = 0;
   Regs.h.bh = 1;
   int86(0x10, &Regs, &Regs);
#else
   inp(INPUT_STATUS_1);
   outp(AC_INDEX, AC_MODE_INDEX);
   outp(AC_DATA_W, inp(AC_DATA_R) | 0x80);
#endif /* USE_BIOS */

   /* We're read to roll; the upper 15 pages are set up, the
      palette RAM is in a pass-through state, and 16-pages-of-16-
      colors paging is enabled. Now we'll loop through and display
      each of pages 15 through 1 and then back to 15 for one frame
      until a key is pressed */
   for ( Page = 15, i = 0 ; i < 1000; i++ ) {

#if USE_BIOS
      /* Select the desired color page */
      Regs.h.ah = 0x10;
      Regs.h.al = 0x13;
      Regs.h.bl = 1;
      Regs.h.bh = Page;
      int86(0x10, &Regs, &Regs);
#else
      /* Wait for the leading edge of the vertical sync pulse; this
         ensures that we change color pages during vertical
         non-display time, and that the page flips are even spaced
         over time */
      while ( (inp(INPUT_STATUS_1) & 0x08) != 0 )
         ;                 /* wait for non-vertical sync time */
      while ( (inp(INPUT_STATUS_1) & 0x08) == 0 )
         ;                 /* wait for vertical sync time */
      inp(INPUT_STATUS_1);
      outp(AC_INDEX, AC_COLOR_SELECT_INDEX);
      outp(AC_DATA_W, Page);
#endif /* USE_BIOS */


      /* Cycle from page 15 down to page 1, and then back to page 15.
         Avoid page 0 entirely */
      if (--Page == 0)
         Page = 15;
   }

   /* Restore text mode and done */
   Regs.x.ax = 0x0003;
   int86(0x10, &Regs, &Regs);
}

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
   *ScreenPtr |= 0xFF;
}

/* Draws a circle of radius Radius in color Color centered at
 * screen coordinate (X,Y) */
void DrawCircle(int X, int Y, int Radius, int Color) {
   int MajorAxis, MinorAxis;
   unsigned long RadiusSqMinusMajorAxisSq;
   unsigned long MinorAxisSquaredThreshold;

   /* Set drawing color via set/reset */
   outpw(GC_INDEX, (0x0F << 8) | SET_RESET_ENABLE_INDEX);
                                 /* enable set/reset for all planes */
   outpw(GC_INDEX, (Color << 8) | SET_RESET_INDEX);
                                 /* set set/reset (drawing) color */
   outp(GC_INDEX, BIT_MASK_INDEX);
                                 /* leave the GC Index reg pointing to
                                    the Bit Mask reg */

   /* Set up to draw the circle by setting the initial point to one
      end of the 1/8th of a circle arc we'll draw */
   MajorAxis = 0;
   MinorAxis = Radius;
   /* Set initial Radius**2 - MajorAxis**2 (MajorAxis is initially 0) */
   RadiusSqMinusMajorAxisSq = Radius * Radius;
   /* Set threshold for minor axis movement at (MinorAxis - 0.5)**2 */
   MinorAxisSquaredThreshold = MinorAxis * MinorAxis - MinorAxis;

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
      /* Advance (Radius**2 - MajorAxis**2); if it equals or passes
         the MinorAxis**2 threshold, advance one pixel along the minor
         axis and set the next MinorAxis**2 threshold */
      if ( (RadiusSqMinusMajorAxisSq -=
         MajorAxis + MajorAxis + 1) <= MinorAxisSquaredThreshold ) {
         MinorAxis--;
         MinorAxisSquaredThreshold -= MinorAxis + MinorAxis;
      }
      MajorAxis++;         /* advance one pixel along the major axis */
   } while ( MajorAxis <= MinorAxis );

   /* Reset the Bit Mask register to normal */
   outp(GC_INDEX + 1, 0xFF);

   /* Turn off set/reset enable */
   outpw(GC_INDEX, (0x00 << 8) | SET_RESET_ENABLE_INDEX);
}

