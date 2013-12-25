/* Global X-Sharp variables. */

#include "polygon.h"

/* Base offset of page to which to draw, initially page 0 */
unsigned int CurrentPageBase = PAGE0_START_OFFSET;

/* Base offsets of pages */
unsigned int PageStartOffsets[2] = {PAGE0_START_OFFSET, PAGE1_START_OFFSET};

/* Page currently being shown, page not currently being shown */
int DisplayedPage, NonDisplayedPage;

/* Clip rectangle; clips to the screen */
int ClipMinX = 0, ClipMinY = 0;
int ClipMaxX = SCREEN_WIDTH, ClipMaxY = SCREEN_HEIGHT;

/* All transforms for all objects are recalculated next time through main
   loop if this is 1 */
int RecalcAllXforms = 1;

/* Total number of objects currently active */
int NumObjects;

/* World->View transform, initialized from floats */
Xform WorldViewXform;

/* Object list start and end */
Object ObjectListStart;
Object ObjectListEnd;

/* Contains flags indicating pending ball moves */
int BallEvent = 0;

