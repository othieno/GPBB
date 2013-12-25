/* Function to tell the BIOS to set up properly sized characters for 25 rows of
 16 pixel high text in 640x400 graphics mode. Call immediately after mode set. 
 Based on a contribution by Bill Lindley. */

#include <dos.h>

void Set640x400()
{
   union REGS regs;

   regs.h.ah = 0x11; /* character generator function */
   regs.h.al = 0x24; /* use ROM 8x16 character set for graphics */
   regs.h.bl = 2;    /* 25 rows */
   int86(0x10, &regs, &regs); /* invoke the BIOS video interrupt
                                 to set up the text */
}

