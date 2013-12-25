/* *** Listing 19.3 ***
 *
 * Draws nested ellipses of varying eccentricities in the VGA's
 * hi-res mode, mode 12h.
 * For VGA only.
 * Compile and link with L19-X.c (where X is 1 or 4) using Borland C++ 4.02:
 *   bcc -ms -el19-3X.exe l19-3.c l19-X.c
 * Checked by Jim Mischel 11/30/94.
 */

#include <dos.h>

main() {
   int XRadius, YRadius, Temp, Color;
   union REGS Regs;

   /* Select VGA's hi-res 640x480 graphics mode, mode 12h */
   Regs.x.ax = 0x0012;
   int86(0x10, &Regs, &Regs);

   /* Draw nested ellipses */
   for ( XRadius = 100, YRadius = 2, Color = 7; YRadius < 240;
         XRadius++, YRadius += 2 ) {
      DrawEllipse(640/2, 480/2, XRadius, YRadius, Color);
      Color = (Color + 1) & 0x0F;   /* cycle through 16 colors */
   }

   /* Wait for a key, restore text mode, and done */
   scanf("%c", &Temp);
   Regs.x.ax = 0x0003;
   int86(0x10, &Regs, &Regs);
}
