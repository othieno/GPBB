/* VGA mode 13h functions for Game of Life.
   Tested with Borland C++ */
#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define TEXT_X_OFFSET   27
#define SCREEN_WIDTH_IN_BYTES 320

/* Width & height in pixels of each cell. */
extern unsigned int magnifier;

/* Mode 13h draw pixel function. Pixels are of width & height
   specified by magnifier. */
void draw_pixel(unsigned int x, unsigned int y, unsigned int color)
{
#define SCREEN_SEGMENT  0xA000
   unsigned char far *screen_ptr;
   int i, j;

   FP_SEG(screen_ptr) = SCREEN_SEGMENT;
   FP_OFF(screen_ptr) =
         y * magnifier * SCREEN_WIDTH_IN_BYTES + x * magnifier;
   for (i=0; i<magnifier; i++) {
      for (j=0; j<magnifier; j++) {
         *(screen_ptr+j) = color;
      }
      screen_ptr += SCREEN_WIDTH_IN_BYTES;
   }
}

/* Mode 13h mode-set function. */
void enter_display_mode()
{
   union REGS regset;

   regset.x.ax = 0x0013;
   int86(0x10, &regset, &regset);
}

/* Text mode mode-set function. */
void exit_display_mode()
{
   union REGS regset;

   regset.x.ax = 0x0003;
   int86(0x10, &regset, &regset);
}

/* Text display function. Offsets text to non-graphics area of
   screen. */
void show_text(int x, int y, char *text)
{
   gotoxy(TEXT_X_OFFSET + x, y);
   puts(text);
}

