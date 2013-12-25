/* Initializes the ball(s) and adds them to the object list. */
#include <stdlib.h>
#include <math.h>
#include "polygon.h"

#define NUM_BALLS 1              /* # of balls to initialize */

#include "balvert0.inc" /* face and vertex definitions for ball */

/* X, Y, Z rotations for balls, in tenths of degrees (angles) */
static RotateControl InitialRotate[NUM_BALLS] = {
   {0,10,0},};

/* Face colors for balls, in RGB forms, with each color component expressed
   as a fraction of full intensity in the range 0.0 to 1.0 */
static ModelColor Colors[NUM_BALLS][NUM_FACES] = {
  {{CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(0)}, {CCOMP(0),CCOMP(0),CCOMP(0)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)},
   {CCOMP(1),CCOMP(1),CCOMP(1)}, {CCOMP(1),CCOMP(1),CCOMP(1)}},
};

/* Shading types for faces of balls */
static int FaceShadings[NUM_BALLS][NUM_FACES] = {
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   TEXTURE_MAPPED_SHADING,
   TEXTURE_MAPPED_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   NO_SHADING,
   TEXTURE_MAPPED_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
   DIFFUSE_SHADING | AMBIENT_SHADING,
};

/* Bear bitmap for texture mapping. Note: texture bitmaps must conform
   to the pixel type of the destination; they are not device independent */
char BearBits[] = {
   60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,
   60,60,20,20,20,60,60,60,60,60,60,60,60,60,60,20,20,20,60,60,
   60,20,20,20,20,20,60,60,60,60,60,60,60,60,20,20,20,20,20,60,
   60,20,20,20,20,20,60,60,20,20,20,20,60,60,20,20,20,20,20,60,
   60,20,20,20,20,20,60,20,20,20,20,20,20,60,20,20,20,20,20,60,
   60,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,60,
   60,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,60,
   60,60,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,60,60,
   60,60,60,20,20,20, 3, 3, 3,20,20, 3, 3, 3,20,20,20,60,60,60,
   60,60,20,20,20,20, 3, 3, 3,20,20, 3, 3, 3,20,20,20,20,60,60,
   60,60,20,20,20,20, 3, 3, 3,20,20, 3, 3, 3,20,20,20,20,60,60,
   60,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,60,
   60,20,20,20,20,20,20,20, 0, 0, 0, 0,20,20,20,20,20,20,20,60,
   60,20,20,20,20,20,20,20, 0, 0, 0, 0,20,20,20,20,20,20,20,60,
   60,20,20,20,20,20,20,20, 0, 0, 0, 0,20,20,20,20,20,20,20,60,
   60,20,20,20,20,20,20,20, 0, 0, 0, 0,20,20,20,20,20,20,20,60,
   60,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,60,
   60,60,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,60,60,
   60,60,60,20,20,20,20,20,20,20,20,20,20,20,20,20,20,60,60,60,
   60,60,60,60,60,20,20,20,20,20,20,20,20,20,20,60,60,60,60,60,
};

/* Pattern bitmap for texture mapping. Note: texture bitmaps must conform
   to the pixel type of the destination; they are not device independent. */
char PatBits[] = {
   12,12,12,12,12,12,12,12,12,
   12,12, 3, 3, 3, 3,12,12,12,
   12, 3,12,12,12,12, 3,12,12,
   12,12, 3,12,12, 3,12,12,12,
   12,12,12, 3, 3,12,12,12,12,
   12,12,12,12,12,12,12,12,12,
};

/* Descriptor for bear texture mapping bitmap */
TextureMap BearTex = { 20, BearBits };

/* Descriptor for bear texture mapping bitmap */
TextureMap PatTex = { 9, PatBits };

/* Vertices for first face's area of BearBits. Note that the texture bitmap
   is set up with a boundary row column around it; this is so that if fixed-
   point imprecision causes the source pointer to wander just off the
   bitmap, appropriate pixels will be found (the boundary pixels are
   duplicates of their neighbors) */
Point BearVerts1[] = {
   { 0, 0 }, {19, 0}, {19, 19}, {0, 19}
};

/* Vertices for first face's area of PatBits */
Point PatVerts1[] = {
   { 0, 0 }, {4, 0}, {4, 5}, {0, 5}
};

/* Vertices for second face's area of PatBits */
Point PatVerts2[] = {
   { 4, 0 }, {8, 0}, {8, 5}, {4, 5}
};

typedef struct {
   FaceNumber;       /* index of face with the following texture */
   TextureMap * TexMap; /* pointer to bitmap for texture mapping, if any */
   Point * TexVerts; /* pointer to list of this polygon's vertices, in
                        TextureMap coordinates. Index n must map to index
                        n + 1 in VertNums, (the + 1 is to skip over the unit
                        normal endpoint in VertNums) */
} TexMap;

TexMap TexMaps[] = {
   { 39, &PatTex,  PatVerts1 },
   { 40, &PatTex,  PatVerts2 },
   { 47, &BearTex, BearVerts1 },
};

#define NUM_TEX_MAPS (sizeof(TexMaps)/sizeof(TexMap))

/* Starting coordinates for balls in world space */
static int BallStartCoords[NUM_BALLS][3] = {
   {0,0,-300},
};
/* Delay counts (speed control) for balls */
static int InitRDelayCounts[NUM_BALLS] = {1};
static int BaseRDelayCounts[NUM_BALLS] = {1};
static int InitMDelayCounts[NUM_BALLS] = {1};
static int BaseMDelayCounts[NUM_BALLS] = {1};

void InitializeBalls()
{
   int i, j, k, TexMapFound;
   PObject *WorkingBall;

   for (i=0; i<NUM_BALLS; i++) {
      if ((WorkingBall = malloc(sizeof(PObject))) == NULL) {
         printf("Couldn't get memory\n"); exit(1); }
      WorkingBall->DrawFunc = DrawPObject;
      WorkingBall->RecalcFunc = XformAndProjectPObject;
      WorkingBall->MoveFunc = RotateAndMoveBall;
      WorkingBall->RecalcXform = 1;
      for (k=0; k<2; k++) {
         WorkingBall->EraseRect[k].Left =
            WorkingBall->EraseRect[k].Top = 0x7FFF;
         WorkingBall->EraseRect[k].Right = 0;
         WorkingBall->EraseRect[k].Bottom = 0;
      }
      WorkingBall->RDelayCount = InitRDelayCounts[i];
      WorkingBall->RDelayCountBase = BaseRDelayCounts[i];
      WorkingBall->MDelayCount = InitMDelayCounts[i];
      WorkingBall->MDelayCountBase = BaseMDelayCounts[i];
      /* Set the object->world xform to none */
      for (j=0; j<3; j++)
         for (k=0; k<4; k++)
            WorkingBall->XformToWorld[j][k] = INT_TO_FIXED(0);
      WorkingBall->XformToWorld[0][0] = 
         WorkingBall->XformToWorld[1][1] =
         WorkingBall->XformToWorld[2][2] = INT_TO_FIXED(1);
      /* Set the initial location */
      for (j=0; j<3; j++) {
          WorkingBall->XformToWorld[j][3] =
                INT_TO_FIXED(BallStartCoords[i][j]);
      }
      /* Initial center coordinates */
      WorkingBall->CenterInView.X = WorkingBall->XformToWorld[0][3];
      WorkingBall->CenterInView.Y = WorkingBall->XformToWorld[1][3];
      WorkingBall->CenterInView.Z = WorkingBall->XformToWorld[2][3];
      WorkingBall->NumVerts = NUM_VERTS;
      WorkingBall->NumRealVerts = NUM_REAL_VERTS;
      WorkingBall->VertexList = Verts;
      WorkingBall->NumFaces = NUM_FACES;
      WorkingBall->Rotate = InitialRotate[i];
      if ((WorkingBall->XformedVertexList =
            malloc(NUM_VERTS*sizeof(Point3))) == NULL) {
         printf("Couldn't get memory\n"); exit(1); }
      if ((WorkingBall->ProjectedVertexList =
            malloc(NUM_VERTS*sizeof(Point3))) == NULL) {
         printf("Couldn't get memory\n"); exit(1); }
      if ((WorkingBall->ScreenVertexList =
            malloc(NUM_VERTS*sizeof(Point))) == NULL) {
         printf("Couldn't get memory\n"); exit(1); }
      if ((WorkingBall->FaceList =
            malloc(NUM_FACES*sizeof(Face))) == NULL) {
         printf("Couldn't get memory\n"); exit(1); }
      /* Initialize the faces */
      for (j=0; j<NUM_FACES; j++) {
         WorkingBall->FaceList[j].VertNums = VertNumList[j];
         WorkingBall->FaceList[j].NumVerts = VertsInFace[j];
         WorkingBall->FaceList[j].FullColor.Red = Colors[i][j].Red;
         WorkingBall->FaceList[j].FullColor.Green = Colors[i][j].Green;
         WorkingBall->FaceList[j].FullColor.Blue = Colors[i][j].Blue;
         if ((WorkingBall->FaceList[j].ShadingType = FaceShadings[i][j])
               == NO_SHADING) {
            /* If no shading is in effect, convert the color from the model
               color to a color index, which we can then use directly */
            WorkingBall->FaceList[j].ColorIndex =
                  ModelColorToColorIndex(&Colors[i][j]);

         } else if (WorkingBall->FaceList[j].ShadingType ==
               TEXTURE_MAPPED_SHADING) {

            /* Search TexMaps for this face's texture mapping info */
            TexMapFound = 0;
            for (k=0; (k<NUM_TEX_MAPS) && !TexMapFound; k++) {
               if (TexMaps[k].FaceNumber == j) {
                  WorkingBall->FaceList[j].TexMap = TexMaps[k].TexMap;
                  WorkingBall->FaceList[j].TexVerts = TexMaps[k].TexVerts;
                  TexMapFound = 1;
               }
            }

            if (!TexMapFound) {
               printf("Error: Texture mapping not found for face %d", j);
               exit(1);
            }

         }
      }
      AddObject((Object *)WorkingBall);
   }
}
