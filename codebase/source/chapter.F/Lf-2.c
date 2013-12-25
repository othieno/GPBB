/* Demonstrates unweighted antialiased drawing in 256 color mode.
   Tested with Borland C++ 4.02 in small model by Jim Mischel 12/16/94.
   Requires:  l25-3.c filcnvx.c l22-4.asm
*/
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "polygon.h"

/* Draws the polygon described by the point list PointList in color
   Color, with all vertices offset by (X,Y), to ScanLineBuffer, at
   double horizontal and vertical resolution */
#define DRAW_POLYGON_DOUBLE_RES(PointList,Color,x,y)        \
   Polygon.Length = sizeof(PointList)/sizeof(struct Point); \
   Polygon.PointPtr = PointTemp;                            \
   /* Double all vertical & horizontal coordinates */       \
   for (k=0; k<sizeof(PointList)/sizeof(struct Point); k++) { \
      PointTemp[k].X = PointList[k].X * 2;                  \
      PointTemp[k].Y = PointList[k].Y * 2;                  \
   }                                                        \
   FillCnvxPolyDrvr(&Polygon, Color, x, y, DrawBandedList);

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define SCREEN_SEGMENT 0xA000
#define SCAN_BAND_WIDTH (SCREEN_WIDTH*2)  /* # of double-res pixels
                                             across scan band */
#define BUFFER_SIZE  (SCREEN_WIDTH*2*2)   /* enough space for one scan
                                           line scanned out at double
                                           resolution horz and vert */
void main(void);
void DrawPixel(int, int, char);
int ColorComponent(int, int);
extern int FillCnvxPolyDrvr(struct PointListHeader *, int, int, int,
   void (*)());
extern void DrawBandedList(struct HLineList *, int);

/* Pointer to buffer in which double-res scanned data will reside */
unsigned char *ScanLineBuffer;
int ScanBandStart, ScanBandEnd;  /* top & bottom of each double-res
                                  band we'll draw to ScanLineBuffer */
int ScanBandWidth = SCAN_BAND_WIDTH;  /* # pixels across scan band */
static char Palette[256*3];

void main()
{
   int i, j, k;
   struct PointListHeader Polygon;
   struct Point PointTemp[4];
   static struct Point Face0[] =
         {{198,138},{211,89},{169,44},{144,89}};
   static struct Point Face1[] =
         {{153,150},{198,138},{144,89},{105,113}};
   static struct Point Face2[] =
         {{169,44},{133,73},{105,113},{144,89}};
   unsigned char Megapixel;
   union REGS regset;
   struct SREGS sregs;

   if ((ScanLineBuffer = malloc(BUFFER_SIZE)) == NULL) {
      printf("Couldn't get memory\n");
      exit(0);
   }

   /* Set the display to VGA mode 13h, 320x200 256-color mode */
   regset.x.ax = 0x0013; int86(0x10, &regset, &regset);

   /* Stack the palette for the desired megapixel effect, with each
      2-bit field representing 1 of 4 double-res pixels in one of four
      colors */
   for (i=0; i<256; i++) {
      Palette[i*3] = ColorComponent(i, 3);   /* red component */
      Palette[i*3+1] = ColorComponent(i, 2); /* green component */
      Palette[i*3+2] = ColorComponent(i, 1); /* blue component */
   }
   regset.x.ax = 0x1012;   /* load palette block BIOS function */
   regset.x.bx = 0;        /* start with palette register 0 */
   regset.x.cx = 256;      /* set all 256 palette registers */
   regset.x.dx = (unsigned int) Palette;
   segread(&sregs);
   sregs.es = sregs.ds;       /* point ES:DX to Palette */
   int86x(0x10, &regset, &regset, &sregs);

   /* Scan out the polygons at double resolution one screen scan line
      at a time (two double-res scan lines at a time) */
   for (i=0; i<SCREEN_HEIGHT; i++) {
      /* Set the band dimensions for this pass */
      ScanBandEnd = (ScanBandStart = i*2) + 1;
      /* Clear the drawing buffer */
      memset(ScanLineBuffer, 0, BUFFER_SIZE);
      /* Draw the current band of the cube to the scan line buffer */
      DRAW_POLYGON_DOUBLE_RES(Face0, 3, 0, 0);
      DRAW_POLYGON_DOUBLE_RES(Face1, 2, 0, 0);
      DRAW_POLYGON_DOUBLE_RES(Face2, 1, 0, 0);

      /* Coalesce the double-res pixels into normal screen pixels
         and draw them */
      for (j=0; j<SCREEN_WIDTH; j++) {
         Megapixel = (ScanLineBuffer[j*2] << 6) +
                     (ScanLineBuffer[j*2+1] << 4) +
                     (ScanLineBuffer[j*2+SCAN_BAND_WIDTH] << 2) +
                     (ScanLineBuffer[j*2+SCAN_BAND_WIDTH+1]);
         DrawPixel(j, i, Megapixel);
      }
   }

   getch();    /* wait for a keypress */

   /* Return to text mode and exit */
   regset.x.ax = 0x0003;   /* AL = 3 selects 80x25 text mode */
   int86(0x10, &regset, &regset);
}

/* Draws a pixel of color Color at (X,Y) in mode 13h */
void DrawPixel(int X, int Y, char Color)
{
   char far *ScreenPtr;

   ScreenPtr = (char far *)MK_FP(SCREEN_SEGMENT, Y*SCREEN_WIDTH+X);
   *ScreenPtr = Color;
}

/* Returns the gamma-corrected value representing the number of
   double-res pixels containing the specified color component in a
   megapixel with the specified value */
int ColorComponent(int Value, int Component)
{
   /* Palette settings for 0%, 25%, 50%, 75%, and 100% brightness,
      assuming a gamma value of 2.3 */
   static int GammaTable[] = {0, 34, 47, 56, 63};
   int i;

   /* Add up the number of double-res pixels of the specified color
      in a megapixel of this value */
   i = (((Value & 0x03) == Component) ? 1 : 0) +
       ((((Value >> 2) & 0x03) == Component) ? 1 : 0) +
       ((((Value >> 4) & 0x03) == Component) ? 1 : 0) +
       ((((Value >> 6) & 0x03) == Component) ? 1 : 0);
   /* Look up brightness of the specified color component in a
      megapixel of this value */
   return GammaTable[i];
}

