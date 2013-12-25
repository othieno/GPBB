/* Draws a bitmap, mapped to a convex polygon (draws a texture-mapped
   polygon). "Convex" means that every vertical line drawn through
   the polygon at any point would cross exactly two active edges
   (neither vertical edges nor zero-length edges count as active
   edges; both are acceptable anywhere in the polygon), and that the
   top & bottom edges never cross. Nonconvex polygons won't be drawn
   properly. Note that scanning is done in columns, to avoid
   per-pixel plane mapping. Can't fail. */

#include <stdio.h>
#include <math.h>
#include "polygon.h"

/* Describes the current location and stepping, in both the source and
   the destination, of an edge. Mirrors structure in DRAWTEXP.C. */
typedef struct {
   int Direction;    /* through edge list; 1 for a right edge (forward
                        through vertex list), -1 for a left edge (backward
                        through vertex list) */
   int RemainingScans;  /* height left to scan out in dest */
   int CurrentEnd;      /* vertex # of end of current edge */
   Fixedpoint SourceX;     /* current X location in source for this edge */
   Fixedpoint SourceY;     /* current Y location in source for this edge */
   Fixedpoint SourceStepX; /* X step in source for X step in dest of 1 */
   Fixedpoint SourceStepY; /* Y step in source for X step in dest of 1 */
                        /* variables used for all-integer Bresenham's-type
                           Y stepping through the dest, needed for precise
                           pixel placement to avoid gaps */
   int DestY;           /* current Y location in dest for this edge */
   int DestYIntStep;    /* whole part of dest Y step per column X step */
   int DestYDirection;  /* -1 or 1 to indicate which way Y steps
                           (left/right) */
   int DestYErrTerm;    /* current error term for dest Y stepping */
   int DestYAdjUp;      /* amount to add to error term per column move */
   int DestYAdjDown;    /* amount to subtract from error term when the
                           error term turns over */
} EdgeScan;

int StepEdge(EdgeScan *);
int SetUpEdge(EdgeScan *, int);
void ScanOutLine(EdgeScan *, EdgeScan *);
int GetImagePixel(char *, int, int, int);

/* Statics to save time that would otherwise be spent passing them to
   subroutines. */
int DestX;
static int MaxVert, NumVerts;
static Point * VertexPtr;
static Point * TexVertsPtr;
char * TexMapBits;
int TexMapWidth;

/* Draws a texture-mapped polygon, given a list of destination polygon
   vertices, a list of corresponding source texture polygon vertices, and a
   pointer to the source texture's descriptor. */
void DrawTexturedPolygon(PointListHeader * Polygon, Point * TexVerts,
   TextureMap * TexMap)
{
   int MinX, MaxX, MinVert, i;
   EdgeScan TopEdge, BottomEdge;

   NumVerts = Polygon->Length;
   VertexPtr = Polygon->PointPtr;
   TexVertsPtr = TexVerts;
   TexMapBits = TexMap->TexMapBits;
   TexMapWidth = TexMap->TexMapWidth;

   /* Nothing to draw if less than 3 vertices */
   if (NumVerts < 3) {
      return;
   }

   /* Scan through the destination polygon vertices and find the left of
      the top and bottom edges, taking advantage of our knowledge that
      vertices run in a clockwise direction (else this polygon wouldn't
      be visible due to backface removal) */
   MinX = 32767;
   MaxX = -32768;
   for (i=0; i<NumVerts; i++) {
      if (VertexPtr[i].X < MinX) {
         MinX = VertexPtr[i].X;
         MinVert = i;
      }
      if (VertexPtr[i].X > MaxX) {
         MaxX = VertexPtr[i].X;
         MaxVert = i;
      }
   }

   /* Reject flat (0-pixel-high) polygons */
   if (MinX >= MaxX) {
      return;
   }

   /* The destination X coordinate is not edge specific; it applies to
      both edges, since we always step X by 1 */
   DestX = MinX;

   /* Set up to scan the initial top and bottom edges of the source and
      destination polygons. We always step the destination polygon edges
      by one in X, so calculate the corresponding destination Y step for
      each edge, and then the corresponding source image Y and X steps */
   TopEdge.Direction = 1;     /* set up top edge first */
   SetUpEdge(&TopEdge, MinVert);
   BottomEdge.Direction = -1; /* set up bottom edge */
   SetUpEdge(&BottomEdge, MinVert);

   /* Step across destination edges one column at a time. At each
      column, find the corresponding edge points in the source image. Scan
      between the edge points in the source, drawing the corresponding
      pixels down the current column in the destination polygon. (We
      know which way the top and bottom edges run through the vertex list
      because visible (non-backface-culled) polygons always have the vertices
      in clockwise order as seen from the viewpoint) */
   for (;;) {
      /* Done if off right of clip rectangle */
      if (DestX >= ClipMaxX) {
         return;
      }

      /* Draw only if inside X bounds of clip rectangle */
      if (DestX >= ClipMinX) {
         /* Draw the column between the two current edges */
         ScanOutLine(&TopEdge, &BottomEdge);
      }

      /* Advance the source and destination polygon edges, ending if we've
         scanned all the way to the right of the polygon */
      if (!StepEdge(&TopEdge)) {
         break;
      }
      if (!StepEdge(&BottomEdge)) {
         break;
      }
      DestX++;
   }
}

/* Steps an edge one column in the destination, and the corresponding
   distance in the source. If an edge runs out, starts a new edge if there
   is one. Returns 1 for success, or 0 if there are no more edges to scan. */
int StepEdge(EdgeScan * Edge)
{
   /* Count off the column we stepped last time; if this edge is
      finished, try to start another one */
   if (--Edge->RemainingScans == 0) {
      /* Set up the next edge; done if there is no next edge */
      if (SetUpEdge(Edge, Edge->CurrentEnd) == 0) {
         return(0);  /* no more edges; done drawing polygon */
      }
      return(1);     /* all set to draw the new edge */
   }

   /* Step the current source edge */
   Edge->SourceY += Edge->SourceStepY;
   Edge->SourceX += Edge->SourceStepX;

   /* Step dest Y with Bresenham-style variables, to get precise dest pixel
      placement and avoid gaps */
   Edge->DestY += Edge->DestYIntStep;  /* whole pixel step */
   /* Do error term stuff for fractional pixel Y step handling */
   if ((Edge->DestYErrTerm += Edge->DestYAdjUp) > 0) {
      Edge->DestY += Edge->DestYDirection;
      Edge->DestYErrTerm -= Edge->DestYAdjDown;
   }

   return(1);
}

/* Sets up an edge to be scanned; the edge starts at StartVert and proceeds
   in direction Edge->Direction through the vertex list. Edge->Direction must
   be set prior to call; -1 to scan a bottom edge (backward through the vertex
   list), 1 to scan a top edge (forward through the vertex list).
   Automatically skips over 0-height edges. Returns 1 for success, or 0 if
   there are no more edges to scan. This is a little tricky because we do
   column-oriented scanning to avoid plane stuff, but still want to have the
   same fill conventions as normal polygons (top and left inclusive, bottom
   and right exclusive). We do this by biasing the edges appropriately:

   edges with non-negative slopes: ceiling(y+1)
   edges with negative slopes: ceiling(y)

   where bottom edge coordinates are non-inclusive for filling, and top
   edge coordinates are inclusive.
 */
int SetUpEdge(EdgeScan * Edge, int StartVert)
{
   int NextVert, DestYHeight;
   Fixedpoint DestXWidth;

   for (;;) {
      /* Done if this edge starts at the rightmost vertex */
      if (StartVert == MaxVert) {
         return(0);
      }

      /* Advance to the next vertex, wrapping if we run off the start or end
         of the vertex list */
      NextVert = StartVert + Edge->Direction;
      if (NextVert >= NumVerts) {
         NextVert = 0;
      } else if (NextVert < 0) {
         NextVert = NumVerts - 1;
      }

      /* Calculate the variables for this edge and done if this is not a
         zero-height edge */
      if ((Edge->RemainingScans =
            VertexPtr[NextVert].X - VertexPtr[StartVert].X) != 0) {
         DestXWidth = INT_TO_FIXED(Edge->RemainingScans);
         Edge->CurrentEnd = NextVert;
         Edge->SourceY = INT_TO_FIXED(TexVertsPtr[StartVert].Y);
         Edge->SourceX = INT_TO_FIXED(TexVertsPtr[StartVert].X);
         Edge->SourceStepY = FixedDiv(INT_TO_FIXED(TexVertsPtr[NextVert].Y) -
               Edge->SourceY, DestXWidth);
         Edge->SourceStepX = FixedDiv(INT_TO_FIXED(TexVertsPtr[NextVert].X) -
               Edge->SourceX, DestXWidth);

         /* Set up Bresenham-style variables for dest Y stepping */
         if ((DestYHeight =
               (VertexPtr[NextVert].Y - VertexPtr[StartVert].Y)) <= 0) {
            /* Set up for drawing bottom to top */
            Edge->DestYDirection = -1;
            DestYHeight = -DestYHeight;
            Edge->DestYIntStep = -(DestYHeight / Edge->RemainingScans);
            Edge->DestY = VertexPtr[StartVert].Y;
         } else {
            /* Set up for drawing top to bottom */
            Edge->DestYDirection = 1;
            Edge->DestYIntStep = DestYHeight / Edge->RemainingScans;
            Edge->DestY = VertexPtr[StartVert].Y + 1;
         }
         Edge->DestYErrTerm = 1 - Edge->RemainingScans;
         Edge->DestYAdjUp = DestYHeight % Edge->RemainingScans;
         Edge->DestYAdjDown = Edge->RemainingScans;
         return(1);  /* success */
      }
      StartVert = NextVert;   /* keep looking for a non-0-height edge */
   }
}

