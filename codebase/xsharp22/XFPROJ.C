/* Transforms all vertices in the specified polygon-based object into
   view space, then perspective projects them to screen space and maps
   them to screen coordinates, storing the results in the object.
   Recalculates object->view transformation because only if the
   transform changes would we bother to retransform the vertices.
   Also recalculates CenterInView and moves the object through the
   object list accordingly.
 */
#include <math.h>
#include "polygon.h"

static Point3 CenterPoint = {0, 0, 0};

void XformAndProjectPObject(PObject * ObjectToXform)
{
   int i, NumPoints = ObjectToXform->NumVerts;
   int NumRealPoints = ObjectToXform->NumRealVerts;
   Point3 * Points = ObjectToXform->VertexList;
   Point3 * XformedPoints = ObjectToXform->XformedVertexList;
   Point3 * ProjectedPoints =
         ObjectToXform->ProjectedVertexList;
   Point * ScreenPoints = ObjectToXform->ScreenVertexList;

   /* Recalculate the object->view transform */
   ConcatXforms(WorldViewXform, ObjectToXform->XformToWorld,
         ObjectToXform->XformToView);

   /* Recalculate CenterInView */
   XformVec(ObjectToXform->XformToView, (Fixedpoint *) &CenterPoint,
         (Fixedpoint *) &ObjectToXform->CenterInView);

   /* Apply that new transformation and project all polygon vertices (not
      unit normal endpoints yet, though) */
   for (i=0; i<NumRealPoints; i++, Points++, XformedPoints++,
         ProjectedPoints++, ScreenPoints++) {
      /* Transform to view space */
      XformVec(ObjectToXform->XformToView, (Fixedpoint *) Points,
            (Fixedpoint *) XformedPoints);

      /* Perspective-project to screen space */
      ProjectedPoints->X =
            FixedMul(FixedDiv(XformedPoints->X, XformedPoints->Z),
            DOUBLE_TO_FIXED(PROJECTION_RATIO * (SCREEN_WIDTH/2)));
      ProjectedPoints->Y =
            FixedMul(FixedDiv(XformedPoints->Y, XformedPoints->Z),
            DOUBLE_TO_FIXED(PROJECTION_RATIO * (SCREEN_WIDTH/2)));
      ProjectedPoints->Z = XformedPoints->Z;

      /* Convert to screen coordinates. The Y coord is negated to
         flip from increasing Y being up to increasing Y being down,
         as expected by the polygon filler. Add in half the screen
         width and height to center on the screen */
      ScreenPoints->X = ((int) ((ProjectedPoints->X +
            DOUBLE_TO_FIXED(0.5)) >> 16)) + SCREEN_WIDTH/2;
      ScreenPoints->Y = (-((int) ((ProjectedPoints->Y +
            DOUBLE_TO_FIXED(0.5)) >> 16))) + SCREEN_HEIGHT/2;
   }

   /* Now transform the unit normal endpoints into view space, but there's
      no reason to waste time getting them into screen space or screen
      coordinates */
   for (i=NumRealPoints; i<NumPoints; i++, Points++, XformedPoints++) {
      /* Transform to view space */
      XformVec(ObjectToXform->XformToView, (Fixedpoint *) Points,
            (Fixedpoint *) XformedPoints);
   }
}

