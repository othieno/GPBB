/* Demonstrates non-antialiased drawing in 640x480 Hicolor (32K color) mode on 
an ET4000-based SuperVGA with a Sierra Hicolor DAC installed. 
  Tested with Borland C++ 4.02 in small model by Jim Mischel 12/16/94. 
  Requires: L26-1.C, FILCNVX.C, L22-4.ASM, and either L26-3.C or L26-4.ASM.
*/

#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include "polygon.h"
/* Draws the polygon described by the point list PointList in color
   Color, with all vertices offset by (x,y) */
#define DRAW_POLYGON(PointList,Color,x,y) {                 \
   Polygon.Length = sizeof(PointList)/sizeof(struct Point); \
   Polygon.PointPtr = PointList;                            \
   FillCnvxPolyDrvr(&Polygon, Color, x, y, DrawHCLineList);}

void main(void);
extern int SetHCMode(int);
extern int FillCnvxPolyDrvr(struct PointListHeader *, int, int, int,
   void (*)());
extern void DrawHCLineList(struct HLineList *, int);
int BitmapWidthInBytes = 640*2; /* # of bytes per raster line */

void main()
{
   struct PointListHeader Polygon;
   static struct Point Face0[] = {{396,276},{422,178},{338,88},{288,178}};
   static struct Point Face1[] = {{306,300},{396,276},{288,178},{210,226}};
   static struct Point Face2[] = {{338,88},{266,146},{210,226},{288,178}};
   union REGS regset;

   /* Attempt to enable 640x480 Hicolor mode */
   if (SetHCMode(0x2E) == 0)
      { printf("No Hicolor DAC detected\n"); exit(0); };

   /* Draw the cube */
   DRAW_POLYGON(Face0, 0x1F, 0, 0);       /* full-intensity blue */
   DRAW_POLYGON(Face1, 0x1F << 5, 0, 0);  /* full-intensity green */
   DRAW_POLYGON(Face2, 0x1F << 10, 0, 0); /* full-intensity red */
   getch();    /* wait for a keypress */

   /* Return to text mode and exit */
   regset.x.ax = 0x0003;   /* AL = 3 selects 80x25 text mode */
   int86(0x10, &regset, &regset);
}

