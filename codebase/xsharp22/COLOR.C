/* Color handling routines. The current color model is the RGB color cube. */

#include <stdlib.h>
#include "polygon.h"

/* Converts a model color (a color in the RGB color cube, in the current
   color model) to a color index for mode X. Pure primary colors are
   special-cased, and everything else is handled by a 2-2-2 model. */
int ModelColorToColorIndex(ModelColor * Color)
{
   if (Color->Red == 0) {
      if (Color->Green == 0) {
         /* Pure blue */
         return(192+(Color->Blue >> 2));
      } else if (Color->Blue == 0) {
         /* Pure green */
         return(128+(Color->Green >> 2));
      }
   } else if ((Color->Green == 0) && (Color->Blue == 0)) {
      /* Pure red */
      return(64+(Color->Red >> 2));
   }
   /* Multi-color mix; look up the index with the two most significant bits
      of each color component */
   return(((Color->Red & 0xC0) >> 2) | ((Color->Green & 0xC0) >> 4) |
         ((Color->Blue & 0xC0) >> 6));
}

/* Adjusts the red, green, and blue components of a color to the specified
   fractions of full intensity, limiting colors to no more than the brightest
   valid color. Note that in this case FixedMul is actually being used to
   multiply colors in 32.0 format by intensities in 16.16 format, generating
   a 32.0 result, of which the 8 lower bits are the adjusted color. Casting
   Source->Red to Fixedpoint effectively just stretches it a long, putting it
   in 32.0 format for FixedMul rather than its normal 8.0 format. */
void IntensityAdjustColor(ModelColor * Dest, ModelColor * Source,
   ModelIntensity * Intensity)
{
   Dest->Red = min(FixedMul((Fixedpoint)Source->Red, Intensity->Red), 255);
   Dest->Green = min(FixedMul((Fixedpoint)Source->Green, Intensity->Green),
         255);
   Dest->Blue = min(FixedMul((Fixedpoint)Source->Blue, Intensity->Blue),
         255);
}

