// try to create a primary buffer and set its format, to configure
// the hardware
#include <windows.h>
#include <dsound.h>

LPDIRECTSOUNDBUFFER SetSoundHWFormat (LPDIRECTSOUND pDS,
   int channels, int bits_per_sample, int samples_per_sec)
{
   DSBUFFERDESC         dsbuf;
   WAVEFORMATEX         format;
   LPDIRECTSOUNDBUFFER  pDSPBuf;

   memset (&format, 0, sizeof(format));
   format.wFormatTag = WAVE_FORMAT_PCM;
   format.nChannels = channels;
   format.wBitsPerSample = bits_per_sample;
   format.nSamplesPerSec = samples_per_sec;
   format.nBlockAlign = format.nChannels*format.wBitsPerSample / 8;
   format.cbSize = 0;
   format.nAvgBytesPerSec = format.nSamplesPerSec*format.nBlockAlign;

   memset (&dsbuf, 0, sizeof(dsbuf));
   dsbuf.dwSize = sizeof(DSBUFFERDESC);
   dsbuf.dwFlags = DSBCAPS_PRIMARYBUFFER;
   dsbuf.dwBufferBytes = 0;
   dsbuf.lpwfxFormat = NULL;

   if (pDS->lpVtbl->CreateSoundBuffer(pDS, &dsbuf, &pDSPBuf, NULL) ==
       DS_OK)
   {
      if (pDSPBuf->lpVtbl->SetFormat (pDSPBuf, &format) == DS_OK)
         return pDSPBuf; // succeeded in configuring hardware
      else
         return NULL;    // didn't succeed in configuring hardware
   }
   else
   {
      return NULL;   // couldn't create a primary buffer
   }
}
