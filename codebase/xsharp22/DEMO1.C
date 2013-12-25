/* Sample X-Sharp 3D animation program. Demonstrates ambient and diffuse
   shading, with up to three spotlights and ambient shading of a white ball.
   The ambient light is green, as are two of the spots; given the palette
   set-up, which is optimized for pure primary colors by assigning 64 levels
   to each primary, shading with all green looks very good. The third
   spotlight is blue; when it's on and any other lighting source is on,
   color quantization problems become apparent; because the palette has only
   four levels of each primary for use in mixing, only very rough
   approximations of mixed colors can be displayed.

   All C code tested with Borland C++ 4.02 in C compilation mode and the
   small model. */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include "polygon.h"

#define FIXED_REPS   0   /* set to # of reps to stop after, or
                            to 0 for free-running */

void DisplayInstructions(void);

void main()
{
   int Done = 0, i, c;
   Object *ObjectPtr;
   union REGS regset;
   unsigned long Reps = 0;

   DisplayInstructions();  /* put up opening instruction screen */

   SetGraphicsMode();      /* set the screen to graphics mode */
   InitializeObjectList(); /* set up the initial objects */
   InitializeFixedPoint(); /* set up fixed-point data */
   InitializeBalls();      /* set up the ball(s) and add them to the
                              object list */
   InitializePalette();    /* set up the palette for whatever color mapping
                              the driver for this adapter uses */
   InitializeLighting();   /* set the initial lighting conditions */

   /* Start off showing page 0 */
   ShowPage(PageStartOffsets[DisplayedPage = 0]);

   /* Keep transforming the objects, drawing them to the undisplayed page,
      and flipping the page to show them */
   do {

      /* For each object, regenerate viewing info, if necessary */
      for (i=0, ObjectPtr = ObjectListStart.NextObject; i<NumObjects;
            i++, ObjectPtr = ObjectPtr->NextObject) {
         if (ObjectPtr->RecalcXform || RecalcAllXforms) {
            ObjectPtr->RecalcFunc(ObjectPtr);
            ObjectPtr->RecalcXform = 0;
         }
      }
      RecalcAllXforms = 0;

      CurrentPageBase =    /* select other page for drawing to */
            PageStartOffsets[NonDisplayedPage = DisplayedPage ^ 1];

      /* For each object, clear the portion of the non-displayed page
         that was drawn to last time, then reset the erase extent */
      for (i=0, ObjectPtr = ObjectListStart.NextObject; i<NumObjects;
            i++, ObjectPtr = ObjectPtr->NextObject) {
         FillRectangleX(ObjectPtr->EraseRect[NonDisplayedPage].Left,
            ObjectPtr->EraseRect[NonDisplayedPage].Top,
            ObjectPtr->EraseRect[NonDisplayedPage].Right,
            ObjectPtr->EraseRect[NonDisplayedPage].Bottom,
            CurrentPageBase, 0);
         ObjectPtr->EraseRect[NonDisplayedPage].Left =
              ObjectPtr->EraseRect[NonDisplayedPage].Top = 0x7FFF;
         ObjectPtr->EraseRect[NonDisplayedPage].Right =
               ObjectPtr->EraseRect[NonDisplayedPage].Bottom = 0;
      }

      /* Sort the objects so we can draw them back to front */
      SortObjects();

      /* Draw all objects */
      for (i=0, ObjectPtr = ObjectListStart.NextObject; i<NumObjects;
            i++, ObjectPtr = ObjectPtr->NextObject)
         ObjectPtr->DrawFunc(ObjectPtr);

      /* Flip to display the page into which we just drew */
      ShowPage(PageStartOffsets[DisplayedPage = NonDisplayedPage]);

      /* Let the user control lighting and ball location */
      while (kbhit()) {
         switch(c = getch()) {
            /* Esc to exit */
            case 0x1B:
               Done = 1;
               break;
            /* Toggle the on/off state of spot 0, 1, or 2 */
            case '0':
            case '1':
            case '2':
               if (GetSpotState(c - '0'))
                  TurnSpotOff(c - '0');
               else
                  TurnSpotOn(c - '0');
               break;
            /* Toggle ambient shading on or off */
            case 'B':
            case 'b':
               if (GetAmbientState())
                  TurnAmbientOff();
               else
                  TurnAmbientOn();
               break;
            case 'A':
            case 'a':
               BallEvent |= MOVE_AWAY;
               break;
            /* Spin the ball around Y rather than X, or vice-versa */
            case 'S':
            case 's':
               BallEvent |= FLIP_SPIN_AXIS;
               break;
            case 'T':
            case 't':
               BallEvent |= MOVE_TOWARD;
               break;
            case 0:     /* extended code */
               switch (getch()) {
                  case 0x4B:     /* ball movement events; record them */
                     BallEvent |= MOVE_LEFT;
                     break;
                  case 0x4D:
                     BallEvent |= MOVE_RIGHT;
                     break;
                  case 0x48:
                     BallEvent |= MOVE_UP;
                     break;
                  case 0x50:
                     BallEvent |= MOVE_DOWN;
                     break;
                  default:
                     break;
               }
               break;
            default:
               break;
         }
      }

      /* Move and reorient each object */
      for (i=0, ObjectPtr = ObjectListStart.NextObject; i<NumObjects;
            i++, ObjectPtr = ObjectPtr->NextObject)
         ObjectPtr->MoveFunc(ObjectPtr);

      /* Count loop passes */
      Reps++;
#if FIXED_REPS
      if (Reps == FIXED_REPS) Done = 1;
#endif

   } while (!Done);
   /* Return to text mode and exit */
   regset.x.ax = 0x0003;   /* AL = 3 selects 80x25 text mode */
   int86(0x10, &regset, &regset);
   exit(1);
}

void DisplayInstructions() {
   printf("Control keys:\n");
   printf(" 0, 1, 2: toggle spotlights 0, 1, and 2 on and off\n");
   printf(" A      : move ball away from you\n");
   printf(" B      : toggle ambient (background) light on and off\n");
   printf(" S      : flip the spin axis from X to Y or Y to X\n");
   printf(" T      : move ball toward you\n");
   printf(" arrows : move ball up, down, left, right\n");
   printf(" Esc    : exit\n");
   printf("\nPress any key to begin...\n");
   if (getch() == 0x1B) exit(0);
}

