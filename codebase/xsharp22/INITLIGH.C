/* Initializes lighting. */

#include "polygon.h"

/* Selects 0-10-0% intensity for R, G, and B for initial ambient lighting */
static ModelIntensity InitialAmbient = {
      DOUBLE_TO_FIXED(0.0), DOUBLE_TO_FIXED(0.10), DOUBLE_TO_FIXED(0.0)
};

/* Initial spot 0 comes in from the upper right */
static Point3 InitialSpot0Direction = {
      DOUBLE_TO_FIXED(-1), DOUBLE_TO_FIXED(-1), DOUBLE_TO_FIXED(-1)
};

/* Initial spot 0 is 0% red, 75% green, 0% blue */
static ModelIntensity InitialSpot0Intensity = {
      DOUBLE_TO_FIXED(0), DOUBLE_TO_FIXED(0.75), DOUBLE_TO_FIXED(0)
};

/* Spot 1 comes in from the lower left */
static Point3 Spot1Direction = {
      DOUBLE_TO_FIXED(1), DOUBLE_TO_FIXED(1), DOUBLE_TO_FIXED(-1)
};

/* Spot 1 is 0% red, 40% green, 0% blue */
static ModelIntensity Spot1Intensity = {
      DOUBLE_TO_FIXED(0.0), DOUBLE_TO_FIXED(0.4), DOUBLE_TO_FIXED(0.0)
};

/* Spot 2 comes in from the left */
static Point3 Spot2Direction = {
      DOUBLE_TO_FIXED(1), DOUBLE_TO_FIXED(0), DOUBLE_TO_FIXED(0)
};

/* Spot 2 is 0% red, 0% green, 60% blue */
static ModelIntensity Spot2Intensity = {
      DOUBLE_TO_FIXED(0.0), DOUBLE_TO_FIXED(0.0), DOUBLE_TO_FIXED(0.6)
};

void InitializeLighting()
{
   SetAmbientIntensity(&InitialAmbient);
   TurnAmbientOn();

   /* Prepare first spotlight for when we'll need it */
   SetSpotDirection(0, &InitialSpot0Direction);
   SetSpotIntensity(0, &InitialSpot0Intensity);

   /* Prepare second spotlight for when we'll need it */
   SetSpotDirection(1, &Spot1Direction);
   SetSpotIntensity(1, &Spot1Intensity);

   /* Prepare third spotlight for when we'll need it */
   SetSpotDirection(2, &Spot2Direction);
   SetSpotIntensity(2, &Spot2Intensity);
}

