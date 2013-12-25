/* VGA mode 13h functions for Game of Life.
   Tested with Borland C++. */
#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define TEXT_X_OFFSET   28
#define SCREEN_WIDTH_IN_BYTES 320

#define SCREEN_SEGMENT  0xA000


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
