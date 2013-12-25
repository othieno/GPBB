/* Lighting control functions. */

#include <stdlib.h>
#include <math.h>
#include "polygon.h"

/* Ambient lighting intensity */
ModelIntensity AmbientIntensity;

/* 1 if ambient shading on, 0 if off */
int AmbientOn;

/* Spotlight direction vectors in world and view coordinates. Spotlights are
   defined in world coordinates, but used in view coordinates.  Spotlight
   directions are reversed in view coordinates, to facilitate incident angle
   calculations via the dot product */
Point3 SpotDirectionWorld[MAX_SPOTS];
Point3 SpotDirectionView[MAX_SPOTS];

/* Spotlight intensities */
ModelIntensity SpotIntensity[MAX_SPOTS];

/* Spotlight on/off statuses, defaulting to off */
int SpotOn[MAX_SPOTS] = {0, 0, 0};

/* Sets the ambient lighting level. */
void SetAmbientIntensity(ModelIntensity * Intensity)
{
   AmbientIntensity = *Intensity;
}

/* Returns the ambient lighting level. */
ModelIntensity * GetAmbientIntensity()
{
   return(&AmbientIntensity);
}

/* Turns ambient shading on */
void TurnAmbientOn()
{
   AmbientOn = 1;
}

/* Turns ambient shading off */
void TurnAmbientOff()
{
   AmbientOn = 0;
}

/* Returns the ambient on/off status (1=on, 0=off). */
int GetAmbientState()
{
   return(AmbientOn);
}

/* Sets the unit vector defining a spotlight's direction. Spotlights are
   considered to be infinitely far away, so the light rays are parallel and
   do not vary in angle anywhere in the scene. Vectors are reversed in view
   space to facilitate angle calculations. The passed-in vector does not
   have to be a unit vector. */
void SetSpotDirection(int SpotNumber, Point3 * SpotVector)
{
   double Xlen, Ylen, Zlen, Length;

   if ((SpotNumber < 0) || (SpotNumber >= MAX_SPOTS))
      return;  /* bad spot number */

   /* Convert the direction vector to a unit vector, so we can do fast
      shading calculations. First, calculate the length of the vector */
   Xlen = FIXED_TO_DOUBLE(SpotVector->X);
   Ylen = FIXED_TO_DOUBLE(SpotVector->Y);
   Zlen = FIXED_TO_DOUBLE(SpotVector->Z);
   Length = sqrt(Xlen*Xlen + Ylen*Ylen + Zlen*Zlen);

   /* Scale it to a unit vector and offset it from the start point. Flip the
      vector's direction for use in shading calculations */
   SpotDirectionWorld[SpotNumber].X = DOUBLE_TO_FIXED(Xlen/Length);
   SpotDirectionWorld[SpotNumber].Y = DOUBLE_TO_FIXED(Ylen/Length);
   SpotDirectionWorld[SpotNumber].Z = DOUBLE_TO_FIXED(Zlen/Length);

   /* Finally, convert the vector into view space, where we'll actually apply
      the shading */
   XformVec(WorldViewXform, (Fixedpoint *) &SpotDirectionWorld[SpotNumber],
         (Fixedpoint *) &SpotDirectionView[SpotNumber]);
   /* Flip the spot direction vector in view space, for ease of shading
      calculations */
   SpotDirectionView[SpotNumber].X = -SpotDirectionView[SpotNumber].X;
   SpotDirectionView[SpotNumber].Y = -SpotDirectionView[SpotNumber].Y;
   SpotDirectionView[SpotNumber].Z = -SpotDirectionView[SpotNumber].Z;
}

/* Sets the intensity of a spotlight. */
void SetSpotIntensity(int SpotNumber, ModelIntensity * Intensity)
{
   ModelIntensity * SpotIntensityPtr;

   if ((SpotNumber < 0) || (SpotNumber >= MAX_SPOTS))
      return;  /* bad spot number */

   SpotIntensity[SpotNumber] = *Intensity;
}

/* Turns on the specified spotlight. */
void TurnSpotOn(int SpotNumber)
{
   if ((SpotNumber < 0) || (SpotNumber >= MAX_SPOTS))
      return;  /* bad spot number */

   SpotOn[SpotNumber] = 1;
}

/* Turns off the specified spotlight. */
void TurnSpotOff(int SpotNumber)
{
   if ((SpotNumber < 0) || (SpotNumber >= MAX_SPOTS))
      return;  /* bad spot number */

   SpotOn[SpotNumber] = 0;
}

/* Returns a spot's unit direction vector in world space. */
Point3 * GetSpotDirection(int SpotNumber)
{
   if ((SpotNumber < 0) || (SpotNumber >= MAX_SPOTS))
      return(NULL);  /* bad spot number */
   return(&SpotDirectionWorld[SpotNumber]);
}

/* Returns a spot's intensity. */
ModelIntensity * GetSpotIntensity(int SpotNumber)
{
   if ((SpotNumber < 0) || (SpotNumber >= MAX_SPOTS))
      return(NULL);  /* bad spot number */
   return(&SpotIntensity[SpotNumber]);
}

/* Returns a spot's on/off status (1=on, 0=off). */
int GetSpotState(int SpotNumber)
{
   if ((SpotNumber < 0) || (SpotNumber >= MAX_SPOTS))
      return(0);  /* bad spot number */
   return(SpotOn[SpotNumber]);
}

