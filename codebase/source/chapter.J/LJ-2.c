// determines whether there's real DirectSound support in the sound
// driver, or just wave-based emulation
#include <windows.h>
#include <dsound.h>

int RealDirectSoundDriver (LPDIRECTSOUND pDS)
{
   DSCAPS   dscaps;

   dscaps.dwSize = sizeof(dscaps);

   if (pDS->lpVtbl->GetCaps (pDS, &dscaps) != DS_OK)
      return 0;   // couldn't get capabilities

   if (dscaps.dwFlags & DSCAPS_EMULDRIVER)
      return 0;   // no real DirectSound driver support, just waves
   else
      return 1;   // real DirectSound driver support
}
