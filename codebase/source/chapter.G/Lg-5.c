/* Demonstrates unweighted antialiased drawing in 640x480 Hicolor (32K color) 
mode. 
  Tested with Borland C++ 4.02 in small model by Jim Mischel 12/16/94.
  Requires: L26-6.C, L26-1.C, L26-3.C, FILCNVX.C, and L22-4.ASM.
*/
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include "polygon.h"
/* Draws the polygon described by the point list PointList in the
   color specified by RED, GREEN, AND BLUE, with all vertices
   offset by (x,y), to ScanLineBuffer, at ResMul multiple of
   horizontal and vertical resolution. The address of ColorTemp is
   cast to an int to satisfy the prototype for FillCnvxPolyDrvr; this
   trick will work only in a small data model */
#define DRAW_POLYGON_HIGH_RES(PointList,RED,GREEN,BLUE,x,y,ResMul) { \
   Polygon.Length = sizeof(PointList)/sizeof(struct Point);          \
   Polygon.PointPtr = PointTemp;                                     \
   /* Multiply all vertical & horizontal coordinates */              \
   for (k=0; k<sizeof(PointList)/sizeof(struct Point); k++) {        \
      PointTemp[k].X = PointList[k].X * ResMul;                      \
      PointTemp[k].Y = PointList[k].Y * ResMul;                      \
   }                                                                 \
   ColorTemp.Red=RED; ColorTemp.Green=GREEN; ColorTemp.Blue=BLUE;    \
   FillCnvxPolyDrvr(&Polygon, (int)&ColorTemp, x, y, DrawBandedList);}
#define SCREEN_WIDTH 640
#define SCREEN_SEGMENT 0xA000

void main(void);
extern void DrawPixel(int, int, char);
extern void DrawBandedList(struct HLineList *, struct RGB *);
extern int SetHCMode(int);

/* Table of gamma corrected mappings of linear color intensities in
   the range 0-255 to the nearest pixel values in the range 0-31,
   assuming a gamma of 2.3 */
static unsigned char ColorMappings[] = {
    0, 3, 4, 4, 5, 6, 6, 6, 7, 7, 8, 8, 8, 8, 9, 9, 9,10,10,10,
   10,10,11,11,11,11,11,12,12,12,12,12,13,13,13,13,13,13,14,14,
   14,14,14,14,14,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,
   17,17,17,17,17,17,17,17,17,18,18,18,18,18,18,18,18,18,19,19,
   19,19,19,19,19,19,19,19,20,20,20,20,20,20,20,20,20,20,20,21,
   21,21,21,21,21,21,21,21,21,21,22,22,22,22,22,22,22,22,22,22,
   22,22,22,23,23,23,23,23,23,23,23,23,23,23,23,24,24,24,24,24,
   24,24,24,24,24,24,24,24,24,25,25,25,25,25,25,25,25,25,25,25,
   25,25,25,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,27,27,
   27,27,27,27,27,27,27,27,27,27,27,27,27,27,28,28,28,28,28,28,
   28,28,28,28,28,28,28,28,28,28,28,29,29,29,29,29,29,29,29,29,
   29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,30,30,30,30,30,
   30,30,30,30,30,30,31,31,31,31,31,31,31,31,31,31};
/* Pointer to buffer in which high-res scanned data will reside */
struct RGB *ScanLineBuffer;
int ScanBandStart, ScanBandEnd;  /* top & bottom of each high-res
                                  band we'll draw to ScanLineBuffer */
int ScanBandWidth;      /* # subpixels across each scan band */
int BitmapWidthInBytes = 640*2;  /* # of bytes per raster line in
                                    Hicolor VGA display memory */
void main()
{
   int i, j, k, m, Red, Green, Blue, jXRes, kXWidth;
   int SubpixelsPerMegapixel;
   unsigned int Megapixel, ResolutionMultiplier;
   long BufferSize;
   struct RGB ColorTemp;
   struct PointListHeader Polygon;
   struct Point PointTemp[4];
   static struct Point Face0[] =
         {{396,276},{422,178},{338,88},{288,178}};
   static struct Point Face1[] =
         {{306,300},{396,276},{288,178},{210,226}};
   static struct Point Face2[] =
         {{338,88},{266,146},{210,226},{288,178}};
   int LeftBound=210, RightBound=422, TopBound=88, BottomBound=300;
   union REGS regset;

   printf("Subpixel resolution multiplier:");
   scanf("%d", &ResolutionMultiplier);
   SubpixelsPerMegapixel = ResolutionMultiplier*ResolutionMultiplier;
   ScanBandWidth = SCREEN_WIDTH*ResolutionMultiplier;

   /* Get enough space for one scan line scanned out at high
      resolution horz and vert (each pixel is 4 bytes) */
   if ((BufferSize = (long)ScanBandWidth*4*ResolutionMultiplier) >
         0xFFFF) {
      printf("Band won't fit in one segment\n"); exit(0); }
   if ((ScanLineBuffer = malloc((int)BufferSize)) == NULL) {
      printf("Couldn't get memory\n"); exit(0); }

   /* Attempt to enable 640x480 Hicolor mode */
   if (SetHCMode(0x2E) == 0)
      { printf("No Hicolor DAC detected\n"); exit(0); };

   /* Scan out the polygons at high resolution one screen scan line at
      a time (ResolutionMultiplier high-res scan lines at a time) */
   for (i=TopBound; i<=BottomBound; i++) {
      /* Set the band dimensions for this pass */
      ScanBandEnd = (ScanBandStart = i*ResolutionMultiplier) +
            ResolutionMultiplier - 1;
      /* Clear the drawing buffer */
      memset(ScanLineBuffer, 0, BufferSize);
      /* Draw the current band of the cube to the scan line buffer */
      DRAW_POLYGON_HIGH_RES(Face0,0xFF,0,0,0,0,ResolutionMultiplier);
      DRAW_POLYGON_HIGH_RES(Face1,0,0xFF,0,0,0,ResolutionMultiplier);
      DRAW_POLYGON_HIGH_RES(Face2,0,0,0xFF,0,0,ResolutionMultiplier);

  /* Coalesce subpixels into normal screen pixels (megapixels) and draw them */
      for (j=LeftBound; j<=RightBound; j++) {
         jXRes = j*ResolutionMultiplier;
         /* For each screen pixel, sum all the corresponding
            subpixels, for each color component */
         for (k=Red=Green=Blue=0; k<ResolutionMultiplier; k++) {
            kXWidth = k*ScanBandWidth;
            for (m=0; m<ResolutionMultiplier; m++) {
               Red += ScanLineBuffer[jXRes+kXWidth+m].Red;
               Green += ScanLineBuffer[jXRes+kXWidth+m].Green;
               Blue += ScanLineBuffer[jXRes+kXWidth+m].Blue;
            }
         }
         /* Calc each color component's average brightness; convert
            that into a gamma corrected portion of a Hicolor pixel,
            then combine the colors into one Hicolor pixel */
         Red = ColorMappings[Red/SubpixelsPerMegapixel];
         Green = ColorMappings[Green/SubpixelsPerMegapixel];
         Blue = ColorMappings[Blue/SubpixelsPerMegapixel];
         Megapixel = (Red << 10) + (Green << 5) + Blue;
         DrawPixel(j, i, Megapixel);
      }
   }
   getch();    /* wait for a keypress */

   /* Return to text mode and exit */
   regset.x.ax = 0x0003;   /* AL = 3 selects 80x25 text mode */
   int86(0x10, &regset, &regset);
}

