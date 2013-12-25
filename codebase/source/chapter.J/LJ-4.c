// functions to start up and shutdown the game configuration for
// the mouse, and to accumulate mouse moves over multiple calls
// during a frame and to calculate the total move for the frame
#include <windows.h>

extern int window_center_x, window_center_y;

// the two 0 values disable the two acceleration thresholds, and the
// 1 value specifies medium mouse speed
static int     originalmouseparms[3], newmouseparms[3] = {0, 0, 1};
static int     mouse_x_accum, mouse_y_accum;
static POINT   current_pos;

int StartGameMouse (void)
{
   if (!SystemParametersInfo (SPI_GETMOUSE, 0, originalmouseparms, 0))
      return 0;

   if (!SystemParametersInfo (SPI_SETMOUSE, 0, newmouseparms, 0))
      return 0;

   ShowCursor (FALSE);
   SetCursorPos (window_center_x, window_center_y);
   return 1;
}

void StopGameMouse (void)
{
   SystemParametersInfo (SPI_SETMOUSE, 0, originalmouseparms, 0);
   ShowCursor (TRUE);
}

void AccumulateGameMouseMove (void)
{
   GetCursorPos (&current_pos);

   mouse_x_accum += current_pos.x - window_center_x;
   mouse_y_accum += current_pos.y - window_center_y;

   // force the mouse to the center, so there's room to move
   SetCursorPos (window_center_x, window_center_y);
}

void GetGameMouseMoveForFrame (int * mouse_x_move, int * mouse_y_move)
{
   GetCursorPos (&current_pos);
   *mouse_x_move = current_pos.x - window_center_x + mouse_x_accum;
   *mouse_y_move = current_pos.y - window_center_y + mouse_y_accum;
   mouse_x_accum = 0;
   mouse_y_accum = 0;

   // force the mouse to the center, so there's room to move
   SetCursorPos (window_center_x, window_center_y);
}
