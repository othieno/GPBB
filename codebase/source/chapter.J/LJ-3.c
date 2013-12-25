// functions to manipulate the desktop video mode
#include <windows.h>

// attempts to set the desktop video mode to the specified
// resolution and bits per pixel
int SetVideoMode (int bpp, int width, int height)
{
   DEVMODE  gdevmode;

   gdevmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
   gdevmode.dmBitsPerPel = bpp;
   gdevmode.dmPelsWidth = width;
   gdevmode.dmPelsHeight = height;

   // CDS_FULLSCREEN keeps icons and windows from being moved to
   // fit within the new dimensions of the desktop
   if (ChangeDisplaySettings (&gdevmode, CDS_FULLSCREEN) !=
          DISP_CHANGE_SUCCESSFUL)
   {
      return 0;
   }

   return 1;
}

// puts back the default desktop video mode
void RestoreDefaultVideoMode (void)
{
   ChangeDisplaySettings (NULL, 0);
}
