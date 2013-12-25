/* Sets up the palette in mode X, to a 2-2-2 general R-G-B organization, with
   64 separate levels each of pure red, green, and blue. This is very good
   for pure colors, but mediocre at best for mixes.

   -------------------------
   |0  0 | Red|Green| Blue |
   -------------------------
    7  6  5  4  3  2  1  0

   -------------------------
   |0  1 |      Red        |
   -------------------------
    7  6  5  4  3  2  1  0

   -------------------------
   |1  0 |     Green       |
   -------------------------
    7  6  5  4  3  2  1  0

   -------------------------
   |1  1 |      Blue       |
   -------------------------
    7  6  5  4  3  2  1  0

   Colors are gamma corrected for a gamma of 2.3 to provide approximately
   even intensity steps on the screen.
*/

#include <dos.h>
#include "polygon.h"

static unsigned char Gamma4Levels[] = { 0, 39, 53, 63 };
static unsigned char Gamma64Levels[] = {
    0, 10, 14, 17, 19, 21, 23, 24, 26, 27, 28, 29, 31, 32, 33, 34,
   35, 36, 37, 37, 38, 39, 40, 41, 41, 42, 43, 44, 44, 45, 46, 46,
   47, 48, 48, 49, 49, 50, 51, 51, 52, 52, 53, 53, 54, 54, 55, 55,
   56, 56, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 62, 62, 63, 63,
};

static unsigned char PaletteBlock[256][3];   /* 256 RGB entries */

void InitializePalette()
{
   int Red, Green, Blue, Index;
   union REGS regset;
   struct SREGS sregset;

   for (Red=0; Red<4; Red++) {
      for (Green=0; Green<4; Green++) {
         for (Blue=0; Blue<4; Blue++) {
            Index = (Red<<4)+(Green<<2)+Blue;
            PaletteBlock[Index][0] = Gamma4Levels[Red];
            PaletteBlock[Index][1] = Gamma4Levels[Green];
            PaletteBlock[Index][2] = Gamma4Levels[Blue];
         }
      }
   }

   for (Red=0; Red<64; Red++) {
      PaletteBlock[64+Red][0] = Gamma64Levels[Red];
      PaletteBlock[64+Red][1] = 0;
      PaletteBlock[64+Red][2] = 0;
   }

   for (Green=0; Green<64; Green++) {
      PaletteBlock[128+Green][0] = 0;
      PaletteBlock[128+Green][1] = Gamma64Levels[Green];
      PaletteBlock[128+Green][2] = 0;
   }

   for (Blue=0; Blue<64; Blue++) {
      PaletteBlock[192+Blue][0] = 0;
      PaletteBlock[192+Blue][1] = 0;
      PaletteBlock[192+Blue][2] = Gamma64Levels[Blue];
   }

   /* Now set up the palette */
   regset.x.ax = 0x1012;   /* set block of DAC registers function */
   regset.x.bx = 0;        /* first DAC location to load */
   regset.x.cx = 256;      /* # of DAC locations to load */
   regset.x.dx = (unsigned int)PaletteBlock; /* offset of array from which
                                                to load RGB settings */
   sregset.es = _DS; /* segment of array from which to load settings */
   int86x(0x10, &regset, &regset, &sregset); /* load the palette block */
}

