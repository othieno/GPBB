/* *** Listing 19.2 ***
 *
 * Draws a series of concentric ellipses that should appear to be
 * circles in the EGA's hi-res mode, mode 10h. (They may not appear
 * to be circles on monitors that don't display mode 10h with the
 * same aspect ratio as the Enhanced Color Display.)
 * For EGA or VGA.
 * Compile and link with L19-X.c (where X is 1 or 4) using Borland C++ 4.02:
 *   bcc -ms -el19-2X.exe l19-2.c l19-X.c
 * Checked by Jim Mischel 11/30/94.
 */

#include <dos.h>

main() {
   int BaseRadius, Temp, Color;
   union REGS Regs;

   /* Select EGA's hi-res 640x350 graphics mode, mode 10h */
   Regs.x.ax = 0x0010;
   int86(0x10, &Regs, &Regs);

   /* Draw concentric ellipses */
   for ( BaseRadius = 2, Color = 7; BaseRadius < 58; BaseRadius++ ) {
      DrawEllipse(640/2, 350/2, BaseRadius*4, BaseRadius*3, Color);
      Color = (Color + 1) & 0x0F;   /* cycle through 16 colors */
   }

   /* Wait for a key, restore text mode, and done */
   scanf("%c", &Temp);
   Regs.x.ax = 0x0003;
   int86(0x10, &Regs, &Regs);
}
