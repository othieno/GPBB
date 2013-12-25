/* *** Listing 18.2 ***
 *
 * Draws a series of concentric circles.
 * For VGA only, because mode 12h is unique to VGA.
 * Compile and link with Borland C++ 4.02 and TASM 4.0 as follows:
 *   bcc -ms l18-1.c l18-2.c  or
 *   bcc -ms l18-3.c l18-2.c l18-4.asm
 * Checked by Jim Mischel 11/30/94.
 */
#include <dos.h>

main() {
   int Radius, Temp, Color, i;
   union REGS Regs;

   /* Select VGA's hi-res 640x480 graphics mode, mode 12h */
   Regs.x.ax = 0x0012;
   int86(0x10, &Regs, &Regs);

   /* Draw 20 sets of concentric circles for timing purposes */
   for (i = 0; i < 20; i++) {
      for ( Radius = 10, Color = 7; Radius < 240; Radius += 2 ) {
         DrawCircle(640/2, 480/2, Radius, Color);
         Color = (Color + 1) & 0x0F;   /* cycle through 16 colors */
      }
   }

   /* Wait for a key, restore text mode, and done */
   scanf("%c", &Temp);
   Regs.x.ax = 0x0003;
   int86(0x10, &Regs, &Regs);
}
