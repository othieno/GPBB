/* Generates the vertices for a ball approximated with polygonal
   facets, in a form suitable for including into a C module. Vertices,
   faces, and supporting info is generated, in the following format:

----------------------------start------------------------------
#define NUM_FACES <number of faces in object>
#define NUM_VERTS <number of vertices in object>
#define NUM_REAL_VERTS <number of vertices in object, excluding unit normal
      endpoints>

Point3 Verts[] = { <vertices in object, with unit normal endpoints coming
      last> };

static int Face0[] = { <unit normal endpoint index, followed by vertex
      indexes for face, clockwise> };
static int Face1[] = { <unit normal endpoint index, followed by vertex
      indexes for face, clockwise> };
				  :
static int Face<n>[] = { <unit normal endpoint index, followed by vertex
      indexes for face, clockwise> };

static int *VertNumList[] = { <ptr to vertex index list per face> };
static int VertsInFace[] = { <# vertices in each face> };

***Note: the first two vertices in the vertex list for each face are always
the end and start points of a unit normal vector for the face, pointing in
the direction from which the face can be seen; these are used for shading.
The second point is also an active vertex***
----------------------------end--------------------------------
*/

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include "polygon.h"

#define PI  3.14159265358979323846
#define DOUBLE_TO_FIXED(x) ((long) (x * 65536.0 + 0.5))

void main(void);
void PrintVertex(struct Point3 *);
void Print4Indexes(int, int, int, int, int);
void Print5Indexes(int, int, int, int, int, int);
void UnitNormal(struct Point3 * Vec, int BaseIndex, int Vec0EndIndex,
   int Vec1EndIndex);

/* Used to rotate around the Y axis by one band's width */
static double YXform[4][4] = {
   {1.0, 0.0, 0.0, 0.0},
   {0.0, 1.0, 0.0, 0.0},
   {0.0, 0.0, 1.0, 0.0},
   {0.0, 0.0, 0.0, 1.0}
};

/* Used to rotate around the Z axis by one band's width */
static double ZXform[4][4] = {
   {1.0, 0.0, 0.0, 0.0},
   {0.0, 1.0, 0.0, 0.0},
   {0.0, 0.0, 1.0, 0.0},
   {0.0, 0.0, 0.0, 1.0}
};

struct Point3 Pointlist[1000];
int NumPoints = 0;

static FILE *OutputFile;   /* where we'll write to */

int FirstUnitNormalIndex;

void main()
{
   int Radius, Bands, i, j, LastIndex, BandsX2, LastBandStartIndex;
   int TopBandStartIndex, BottomBandStartIndex, FaceNum;
   struct Point3 BaseVec, BandVec, WorkingVec, TempVec;
   char OutputFilename[130];
   char Description[130];

   printf("Radius: ");
   scanf("%d",&Radius);
   printf("Bands: ");
   scanf("%d",&Bands);
   printf("Output file: ");
   OutputFilename[0] = 127;
   cgets(OutputFilename);
   printf("\nBrief description: ");
   Description[0] = 127;
   cgets(Description);
   printf("\n");

   BandsX2 = Bands*2;

   if ((OutputFile = fopen(&OutputFilename[2], "w")) == NULL) {
      printf("Error\n");
      exit(1);
   }

   /* Descriptive comments */
   fprintf(OutputFile, "/* %s */\n", &Description[2]);
   fprintf(OutputFile, "/* Created with radius = %d, bands = %d */\n",
         Radius, Bands);

   /* Defines for # of faces and vertices */
   fprintf(OutputFile, "#define NUM_FACES %d\n", BandsX2*Bands);
   fprintf(OutputFile, "#define NUM_VERTS %d\n\n",
         2+1+BandsX2*(Bands-1)+1+ (BandsX2*Bands));
   /* # of vertices excluding unit normal endpoints */
   fprintf(OutputFile, "#define NUM_REAL_VERTS %d\n\n",
         2+1+BandsX2*(Bands-1)+1);

   /* Do the polygon vertices */
   fprintf(OutputFile, "Point3 Verts[] = {\n");

	/* Generate the rotation matrices */
   AppendRotationY(YXform, PI / Bands);
   AppendRotationZ(ZXform, PI / Bands);

   /* Do the point at the top */
   BaseVec.X = 0.0;
   BaseVec.Y = Radius;
   BaseVec.Z = 0.0;
   BaseVec.W = 1.0;
   PrintVertex(&BaseVec);

   BandVec = BaseVec;

   /* Do the vertices in each band in turn */
   for (i=1; i<Bands; i++) {
      /* Rotate around Z to the next band's latitude */
      XformVec(ZXform, (double *)&BandVec, (double *)&TempVec);
      WorkingVec = BandVec = TempVec;
		/* Do the vertices in this band */
      for (j=0; j<BandsX2; j++) {
         WorkingVec = TempVec;
         PrintVertex(&WorkingVec);
         /* Now rotate around Y to the next vertex's longitude */
         XformVec(YXform, (double *)&WorkingVec, (double *)&TempVec);
      }
   }

   /* Do the point at the bottom */
   BaseVec.Y = -Radius;
   PrintVertex(&BaseVec);

   /* Now generate the unit normal endpoint for each face, as the last
      entries in the vertex list */
   /* Index in overall vertex list of first unit normal endpoint we'll
      create; remember where we can find it */
   FirstUnitNormalIndex = NumPoints;

   FaceNum = 0;

   /* Unit normal endpoints in top band */
   for (i=0; i<BandsX2; i++) {
      UnitNormal(&BaseVec, 0, ((i+1)%BandsX2)+1, i+1);
      PrintVertex(&BaseVec);
      FaceNum++;
   }

   /* Unit normal endpoints in middle bands */
   for (j=0; j<(Bands-2); j++) {
      TopBandStartIndex = j*BandsX2 + 1;
      BottomBandStartIndex = (j+1)*BandsX2 + 1;
		/* Indexes in this band */
      for (i=0; i<BandsX2; i++) {
         UnitNormal(&BaseVec, i+TopBandStartIndex,
               ((i+1)%BandsX2)+TopBandStartIndex,
               i+BottomBandStartIndex);
         PrintVertex(&BaseVec);
         FaceNum++;
      }
   }

   /* Unit normal endpoints in bottom band */
   LastIndex = BandsX2*(Bands-1)+1;
   LastBandStartIndex = BandsX2*(Bands-2)+1;
   for (i=0; i<BandsX2; i++) {
      UnitNormal(&BaseVec, LastBandStartIndex+i,
            LastBandStartIndex+((i+1)%BandsX2), LastIndex);
      PrintVertex(&BaseVec);
      FaceNum++;
   }

   /* Done generating points, including both polygon vertices and unit
      normals */
   fprintf(OutputFile, "};\n\n");


   /* Do the vertex indexes for each face in each band */
   FaceNum = 0;

   /* Vertex indexes in top band, with unit normal endpoint first */
   for (i=0; i<BandsX2; i++) {
      Print4Indexes(FaceNum++, FirstUnitNormalIndex++, 0,
            ((i+1)%BandsX2)+1, i+1);
   }

   /* Vertex indexes in middle bands, with unit normal endpoints first */
   for (j=0; j<(Bands-2); j++) {
      TopBandStartIndex = j*BandsX2 + 1;
      BottomBandStartIndex = (j+1)*BandsX2 + 1;
		/* Indexes in this band */
      for (i=0; i<BandsX2; i++) {
         Print5Indexes(FaceNum++, FirstUnitNormalIndex++,
               i+TopBandStartIndex,
               ((i+1)%BandsX2)+TopBandStartIndex,
               ((i+1)%BandsX2)+BottomBandStartIndex,
               i+BottomBandStartIndex);
      }
   }

   /* Vertex indexes in bottom band, with unit normal endpoint first */
   LastIndex = BandsX2*(Bands-1)+1;
   LastBandStartIndex = BandsX2*(Bands-2)+1;
   for (i=0; i<BandsX2; i++) {
      Print4Indexes(FaceNum++, FirstUnitNormalIndex++,
            LastBandStartIndex+i,
            LastBandStartIndex+((i+1)%BandsX2), LastIndex);
   }

   /* Do the list of pointers to index arrays for each face */
   fprintf(OutputFile, "\nstatic int *VertNumList[] = {\n");
   for (i=0; i<(BandsX2*Bands); i++) {
      fprintf(OutputFile, "Face%d,\n", i);
   }
   fprintf(OutputFile, "};\n");

   /* Do the # of vertices in each face (3 for the top and bottom
      bands, 4 for the rest) */
   fprintf(OutputFile, "\nstatic int VertsInFace[] = {\n");
   for (i=0; i<BandsX2; i++) fprintf(OutputFile, "3,\n");
   for (i=0; i<(BandsX2*(Bands-2)); i++) fprintf(OutputFile, "4,\n");
   for (i=0; i<BandsX2; i++) fprintf(OutputFile, "3,\n");
   fprintf(OutputFile, "};\n");

   exit(0);
}

/* Prints the array of indexes for a 5-vertex face (generally four vertices
   plus the unit normal endpoint, actually). */
void Print5Indexes(int FaceNum, int V1, int V2, int V3, int V4, int V5)
{
      fprintf(OutputFile, "static int Face%d[] = {%d,%d,%d,%d,%d};\n",
            FaceNum, V1, V2, V3, V4, V5);
}

/* Prints the array of indexes for a 4-vertex face (generally three vertices
   plus the unit normal endpoint, actually). */
void Print4Indexes(int FaceNum, int V1, int V2, int V3, int V4)
{
      fprintf(OutputFile, "static int Face%d[] = {%d,%d,%d,%d};\n",
            FaceNum, V1, V2, V3, V4);
}

/* Prints a vertex, in 16.16 fixed-point form */
void PrintVertex(struct Point3 *Vec)
{
   long X = DOUBLE_TO_FIXED(Vec->X);
   long Y = DOUBLE_TO_FIXED(Vec->Y);
   long Z = DOUBLE_TO_FIXED(Vec->Z);
   Pointlist[NumPoints++] = *Vec; /* remember this point */

   fprintf(OutputFile, "{%ld, %ld, %ld},\n", X, Y, Z);
}

/* Calculates the end of the unit normal by calculating the cross-product of
   the first and last vectors, and normalizing it to 1 unit in length */
void UnitNormal(struct Point3 * Vec, int BaseIndex, int Vec0EndIndex,
   int Vec1EndIndex)
{
   double v1, v2, v3, w1, w2, w3, Xlen, Ylen, Zlen, Length;

   v1 = Pointlist[Vec0EndIndex].X - Pointlist[BaseIndex].X;
   v2 = Pointlist[Vec0EndIndex].Y - Pointlist[BaseIndex].Y;
   v3 = Pointlist[Vec0EndIndex].Z - Pointlist[BaseIndex].Z;
   w1 = Pointlist[Vec1EndIndex].X - Pointlist[BaseIndex].X;
   w2 = Pointlist[Vec1EndIndex].Y - Pointlist[BaseIndex].Y;
   w3 = Pointlist[Vec1EndIndex].Z - Pointlist[BaseIndex].Z;
   /* Calculate perpendicular relative to the BaseIndex point in Vec */
   Vec->X = v2*w3 - v3*w2;
   Vec->Y = v3*w1 - v1*w3;
   Vec->Z = v1*w2 - v2*w1;
   /* Calculate the length of the normal, and reverse its direction so that
      a positive normal points toward the screen, negative points away */
   Xlen = -(Vec->X - Pointlist[BaseIndex].X);
   Ylen = -(Vec->Y - Pointlist[BaseIndex].Y);
   Zlen = -(Vec->Z - Pointlist[BaseIndex].Z);
   Length = sqrt(Xlen*Xlen + Ylen*Ylen + Zlen*Zlen);
   /* Scale it to a unit normal and offset it from the start point */
   Vec->X = Pointlist[BaseIndex].X + (Xlen / Length);
   Vec->Y = Pointlist[BaseIndex].Y + (Ylen / Length);
   Vec->Z = Pointlist[BaseIndex].Z + (Zlen / Length);
}

