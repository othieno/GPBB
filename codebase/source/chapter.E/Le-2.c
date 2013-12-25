/* *** Listing 20.2 ***
 *
 * Draws ellipses of varying eccentricities in the VGA's hi-res mode,
 * mode 12h.
 * For VGA only.
 * Compile and link with Borland C++ 4.02 and TASM 4.0 as follows:
 *   bcc -ms l20-1.c l20-2.c  or
 *   bcc -ms l20-3.c l20-2.c l20-4.asm
 * Checked by Jim Mischel 11/30/94.
 */

#include <dos.h>
#include <stdio.h>

main() {
   int XRadius, YRadius, Temp, Color, i;
   union REGS Regs;

   /* Select VGA's hi-res 640x480 graphics mode, mode 12h */
   Regs.x.ax = 0x0012;
   int86(0x10, &Regs, &Regs);

   /* Repeat 10 times */
   for ( i = 0; i < 10; i++ ) {
      /* Draw nested ellipses */
      for ( XRadius = 319, YRadius = 1, Color = 7; YRadius < 240;
            XRadius -= 1, YRadius += 2 ) {
         DrawEllipse(640/2, 480/2, XRadius, YRadius, Color);
         Color = (Color + 1) & 0x0F;   /* cycle through 16 colors */
      }
   }

   /* Wait for a key, restore text mode, and done */
   scanf("%c", &Temp);
   Regs.x.ax = 0x0003;
   int86(0x10, &Regs, &Regs);
}
