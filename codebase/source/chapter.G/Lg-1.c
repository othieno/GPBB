/* Looks for a Sierra Hicolor DAC; if one is present, puts the VGA into the 
specified Hicolor (32K color) mode. Relies on the Tseng Labs ET4000 BIOS and 
hardware; probably will not work on adapters built around other VGA chips. 
Returns 1 for success, 0 for failure; failure can result from no Hicolor DAC, 
too little display memory, or lack of an ET4000.

Tested with Borland C++ 4.02 in small model by Jim Mischel 12/16/94. */

#include <dos.h>
#define DAC_MASK  0x3C6 /* DAC pixel mask reg address, also Sierra
                           command reg address when enabled */
#define DAC_WADDR 0x3C8  /* DAC write address reg address */

/* Mode selections: 0x2D=640x350; 0x2E=640x480; 0x2F=640x400; 0x30=800x600 */
int SetHCMode(int Mode) {
   int i, Temp1, Temp2, Temp3;
   union REGS regset;

   /* See if a Sierra SC1148X Hicolor DAC is present, by trying to
   program and then read back the DAC's command register. (Shouldn't be 
   necessary when using the BIOS Get DAC Type function, but the BIOS function 
   locks up some computers, so it's safer to check the hardware first) */
   inp(DAC_WADDR); /* reset the Sierra command reg enable sequence */
   for (i=0; i<4; i++) inp(DAC_MASK); /* enable command reg access */
   outp(DAC_MASK, 0x00); /* set command reg (if present) to 0x00, and
                            reset command reg enable sequence */
   outp(DAC_MASK, 0xFF); /* command reg access no longer enabled;
                            set pixel mask register to 0xFF */
   for (i=0; i<4; i++) inp(DAC_MASK); /* enable command reg access */
   /* If this is a Hicolor DAC, we should read back the 0 in the
      command reg; otherwise we get the 0xFF in the pixel mask reg */
   i = inp(DAC_MASK); inp(DAC_WADDR); /* reset enable sequence */
   if (i == 0xFF) return(0);

   /* Check for a Tseng Labs ET4000 by poking unique regs, (assumes
      VGA configured for color, w/CRTC addressing at 3D4/5) */
   outp(0x3BF, 3); outp(0x3D8, 0xA0);  /* unlock extended registers */
   /* Try toggling AC R16 bit 4 and seeing if it takes */
   inp(0x3DA); outp(0x3C0, 0x16 | 0x20);
   outp(0x3C0, ((Temp1 = inp(0x3C1)) | 0x10)); Temp2 = inp(0x3C1);
   outp(0x3C0, 0x16 | 0x20); outp(0x3C0, (inp(0x3C1) & ~0x10));
   Temp3 = inp(0x3C1); outp(0x3C0, 0x16 | 0x20);
   outp(0x3C0, Temp1);  /* restore original AC R16 setting */
   /* See if the bit toggled; if so, it's an ET3000 or ET4000 */
   if ((Temp3 & 0x10) || !(Temp2 & 0x10)) return(0);
   outp(0x3D4, 0x33); Temp1 = inp(0x3D5); /* get CRTC R33 setting */
   outp(0x3D5, 0x0A); Temp2 = inp(0x3D5); /* try writing to CRTC */
   outp(0x3D5, 0x05); Temp3 = inp(0x3D5); /*  R33 */
   outp(0x3D5, Temp1);  /* restore original CRTC R33 setting */
   /* If the register was writable, it's an ET4000 */
   if ((Temp3 != 0x05) || (Temp2 != 0x0A)) return(0);

   /* See if a Sierra SC1148X Hicolor DAC is present by querying the
      (presumably) ET4000-compatible BIOS. Not really necessary after
      the hardware check above, but generally more useful; in the
      future it will return information about other high-color DACs */
   regset.x.ax = 0x10F1;   /* Get DAC Type BIOS function # */
   int86(0x10, &regset, &regset); /* ask BIOS for the DAC type */
   if (regset.x.ax != 0x0010) return(0); /* function not supported */
   switch (regset.h.bl) {
      case 0:  return(0);  /* normal DAC (non-Hicolor) */
      case 1:  break;      /* Sierra SC1148X 15-bpp Hicolor DAC */
      default: return(0);  /* other high-color DAC */
   }

   /* Set Hicolor mode */
   regset.x.ax = 0x10F0;   /* Set High-Color Mode BIOS function # */
   regset.h.bl = Mode;     /* desired resolution */
   int86(0x10, &regset, &regset); /* have BIOS enable Hicolor mode */
   return (regset.x.ax == 0x0010); /* 1 for success, 0 for failure */
}

