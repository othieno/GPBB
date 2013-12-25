/* Run-length slice line drawing implementation for mode 0x13, the VGA's
320x200 256-color mode. Not optimized! 
  Compile Borland C++ 4.02 in small model and link with L15-2.C
  Checked by Jim Mischel 11/30/94 */

#include <dos.h>

#define SCREEN_WIDTH    320
#define SCREEN_SEGMENT  0xA000

void DrawHorizontalRun(char far **ScreenPtr, int XAdvance, int RunLength,
                       int Color);
void DrawVerticalRun(char far **ScreenPtr, int XAdvance, int RunLength,
                       int Color);
/* Draws a line between the specified endpoints in color Color. */
void LineDraw(int XStart, int YStart, int XEnd, int YEnd, int Color)
{
   int Temp, AdjUp, AdjDown, ErrorTerm, XAdvance, XDelta, YDelta;
   int WholeStep, InitialPixelCount, FinalPixelCount, i, RunLength;
   char far *ScreenPtr;

   /* We'll always draw top to bottom, to reduce the number of cases we have to
   handle, and to make lines between the same endpoints draw the same pixels */
   if (YStart > YEnd) {
      Temp = YStart;
      YStart = YEnd;
      YEnd = Temp;
      Temp = XStart;
      XStart = XEnd;
      XEnd = Temp;
   }
   /* Point to the bitmap address first pixel to draw */
   ScreenPtr = MK_FP(SCREEN_SEGMENT, YStart * SCREEN_WIDTH + XStart);

   /* Figure out whether we're going left or right, and how far we're
      going horizontally */
   if ((XDelta = XEnd - XStart) < 0)
   {
      XAdvance = -1;
      XDelta = -XDelta;
   }
   else
   {
      XAdvance = 1;
   }
   /* Figure out how far we're going vertically */
   YDelta = YEnd - YStart;

   /* Special-case horizontal, vertical, and diagonal lines, for speed
      and to avoid nasty boundary conditions and division by 0 */
   if (XDelta == 0)
   {
      /* Vertical line */
      for (i=0; i<=YDelta; i++)
      {
         *ScreenPtr = Color;
         ScreenPtr += SCREEN_WIDTH;
      }
      return;
   }
   if (YDelta == 0)
   {
      /* Horizontal line */
      for (i=0; i<=XDelta; i++)
      {
         *ScreenPtr = Color;
         ScreenPtr += XAdvance;
      }
      return;
   }
   if (XDelta == YDelta)
   {
      /* Diagonal line */
      for (i=0; i<=XDelta; i++)
      {
         *ScreenPtr = Color;
         ScreenPtr += XAdvance + SCREEN_WIDTH;
      }
      return;
   }

   /* Determine whether the line is X or Y major, and handle accordingly */
   if (XDelta >= YDelta)
   {
      /* X major line */
      /* Minimum # of pixels in a run in this line */
      WholeStep = XDelta / YDelta;

      /* Error term adjust each time Y steps by 1; used to tell when one
         extra pixel should be drawn as part of a run, to account for
         fractional steps along the X axis per 1-pixel steps along Y */
      AdjUp = (XDelta % YDelta) * 2;

      /* Error term adjust when the error term turns over, used to factor
         out the X step made at that time */
      AdjDown = YDelta * 2;

      /* Initial error term; reflects an initial step of 0.5 along the Y
         axis */
      ErrorTerm = (XDelta % YDelta) - (YDelta * 2);

      /* The initial and last runs are partial, because Y advances only 0.5
         for these runs, rather than 1. Divide one full run, plus the
         initial pixel, between the initial and last runs */
      InitialPixelCount = (WholeStep / 2) + 1;
      FinalPixelCount = InitialPixelCount;

      /* If the basic run length is even and there's no fractional
         advance, we have one pixel that could go to either the initial
         or last partial run, which we'll arbitrarily allocate to the
         last run */
      if ((AdjUp == 0) && ((WholeStep & 0x01) == 0))
      {
         InitialPixelCount--;
      }
      /* If there're an odd number of pixels per run, we have 1 pixel that can't
         be allocated to either the initial or last partial run, so we'll add 0.5
         to error term so this pixel will be handled by the normal full-run loop */
         if ((WholeStep & 0x01) != 0)
      {
         ErrorTerm += YDelta;
      }
      /* Draw the first, partial run of pixels */
      DrawHorizontalRun(&ScreenPtr, XAdvance, InitialPixelCount, Color);
      /* Draw all full runs */
      for (i=0; i<(YDelta-1); i++)
      {
         RunLength = WholeStep;  /* run is at least this long */
         /* Advance the error term and add an extra pixel if the error
            term so indicates */
         if ((ErrorTerm += AdjUp) > 0)
         {
            RunLength++;
            ErrorTerm -= AdjDown;   /* reset the error term */
         }
         /* Draw this scan line's run */
         DrawHorizontalRun(&ScreenPtr, XAdvance, RunLength, Color);
      }
      /* Draw the final run of pixels */
      DrawHorizontalRun(&ScreenPtr, XAdvance, FinalPixelCount, Color);
      return;
   }
   else
   {
      /* Y major line */

      /* Minimum # of pixels in a run in this line */
      WholeStep = YDelta / XDelta;

      /* Error term adjust each time X steps by 1; used to tell when 1 extra
         pixel should be drawn as part of a run, to account for
         fractional steps along the Y axis per 1-pixel steps along X */
      AdjUp = (YDelta % XDelta) * 2;

      /* Error term adjust when the error term turns over, used to factor
         out the Y step made at that time */
      AdjDown = XDelta * 2;

      /* Initial error term; reflects initial step of 0.5 along the X axis */
      ErrorTerm = (YDelta % XDelta) - (XDelta * 2);

      /* The initial and last runs are partial, because X advances only 0.5
         for these runs, rather than 1. Divide one full run, plus the
         initial pixel, between the initial and last runs */
      InitialPixelCount = (WholeStep / 2) + 1;
      FinalPixelCount = InitialPixelCount;

      /* If the basic run length is even and there's no fractional advance, we
         have 1 pixel that could go to either the initial or last partial run,
         which we'll arbitrarily allocate to the last run */
      if ((AdjUp == 0) && ((WholeStep & 0x01) == 0))
      {
         InitialPixelCount--;
      }
      /* If there are an odd number of pixels per run, we have one pixel
         that can't be allocated to either the initial or last partial
         run, so we'll add 0.5 to the error term so this pixel will be
         handled by the normal full-run loop */
      if ((WholeStep & 0x01) != 0)
      {
         ErrorTerm += XDelta;
      }
      /* Draw the first, partial run of pixels */
      DrawVerticalRun(&ScreenPtr, XAdvance, InitialPixelCount, Color);

      /* Draw all full runs */
      for (i=0; i<(XDelta-1); i++)
      {
         RunLength = WholeStep;  /* run is at least this long */
         /* Advance the error term and add an extra pixel if the error
            term so indicates */
         if ((ErrorTerm += AdjUp) > 0)
         {
            RunLength++;
            ErrorTerm -= AdjDown;   /* reset the error term */
         }
         /* Draw this scan line's run */
         DrawVerticalRun(&ScreenPtr, XAdvance, RunLength, Color);
      }
      /* Draw the final run of pixels */
      DrawVerticalRun(&ScreenPtr, XAdvance, FinalPixelCount, Color);
      return;
   }
}
/* Draws a horizontal run of pixels, then advances the bitmap pointer to
   the first pixel of the next run. */
void DrawHorizontalRun(char far **ScreenPtr, int XAdvance,
   int RunLength, int Color)
{
   int i;
   char far *WorkingScreenPtr = *ScreenPtr;

   for (i=0; i<RunLength; i++)
   {
      *WorkingScreenPtr = Color;
      WorkingScreenPtr += XAdvance;
   }
   /* Advance to the next scan line */
   WorkingScreenPtr += SCREEN_WIDTH;
   *ScreenPtr = WorkingScreenPtr;
}
/* Draws a vertical run of pixels, then advances the bitmap pointer to
   the first pixel of the next run. */
void DrawVerticalRun(char far **ScreenPtr, int XAdvance,
   int RunLength, int Color)
{
   int i;
   char far *WorkingScreenPtr = *ScreenPtr;

   for (i=0; i<RunLength; i++)
   {
      *WorkingScreenPtr = Color;
      WorkingScreenPtr += SCREEN_WIDTH;
   }
   /* Advance to the next column */
   WorkingScreenPtr += XAdvance;
   *ScreenPtr = WorkingScreenPtr;
}

