/* POLYGON.H: Header file for filled polygon drawing and 3D animation. */

#define MAX_OBJECTS  100   /* max simultaneous # objects supported */
#define MAX_POLY_LENGTH 4  /* four vertices is the max per poly */
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define PAGE0_START_OFFSET 0
#define PAGE1_START_OFFSET (((long)SCREEN_HEIGHT*SCREEN_WIDTH)/4)

/* Ball movement flags */
#define MOVE_LEFT       0x0001
#define MOVE_RIGHT      0x0002
#define MOVE_UP         0x0004
#define MOVE_DOWN       0x0008
#define MOVE_TOWARD     0x0010
#define MOVE_AWAY       0x0020
#define FLIP_SPIN_AXIS  0x0040

/* Maximum number of simultaneously active spotlights */
#define MAX_SPOTS 3

/* Ratio: distance from viewpoint to projection plane / width of
   projection plane. Defines the width of the field of view. Lower
   absolute values = wider fields of view; higher values = narrower */
#define PROJECTION_RATIO -2.0 /* negative because visible Z
                                 coordinates are negative */

/* Draws the polygon described by the point list PointList in color
   Color with all vertices offset by (X,Y) */
#define DRAW_POLYGON(PointList,NumPoints,Color,X,Y)          \
   Polygon.Length = NumPoints; Polygon.PointPtr = PointList; \
   FillConvexPolygon(&Polygon, Color, X, Y);

/* Draws the polygon described by the point list PointList with a bitmap
   texture mapped onto it */
#define DRAW_TEXTURED_POLYGON(PointList,NumPoints,TexVerts,TexMap) \
   Polygon.Length = NumPoints; Polygon.PointPtr = PointList;       \
   DrawTexturedPolygon(&Polygon, TexVerts, TexMap);

#define INT_TO_FIXED(x) (((long)(int)x) << 16)
#define DOUBLE_TO_FIXED(x) ((long) (x * 65536.0 + 0.5))
#define FIXED_TO_DOUBLE(x) (((double)x) / 65536.0)
#define FIXED_TO_INT(FixedVal) ((int) (FixedVal >> 16))
#define ROUND_FIXED_TO_INT(FixedVal) \
   ((int) ((FixedVal + DOUBLE_TO_FIXED(0.5)) >> 16))

/* Sets a color intensity to the specified levels */
#define SET_INTENSITY(IntensityTemp, R, G, B) \
   IntensityTemp.Red = DOUBLE_TO_FIXED(R);       \
   IntensityTemp.Green = DOUBLE_TO_FIXED(G);     \
   IntensityTemp.Blue = DOUBLE_TO_FIXED(B);

/* Converts a color component value between 0 and 1 to an R, G, or B
   value between 0 and 255. Allows expressing color components as fractions
   in the range 0 and 1, although internally (in ModelColor structures)
   they're actually represented as values between 0 and 255 */
#define CCOMP(Color) ((Color<0) ? 0 : \
   ((Color>=1) ? 255 : ((double)Color*255.0+0.5)))

/* Calculates dot product */
#define DOT_PRODUCT(V1,V2) \
   (FixedMul(V1.X,V2.X)+FixedMul(V1.Y,V2.Y)+FixedMul(V1.Z,V2.Z))

/* Retrieves the specified pixel from the specified image bitmap of the
   specified width. */
#define GET_IMAGE_PIXEL(TexMapBits, TexMapWidth, X, Y) \
   TexMapBits[(Y * TexMapWidth) + X]

/* Masks to mark shading types in Face structure */
#define NO_SHADING      0x0000
#define AMBIENT_SHADING 0x0001
#define DIFFUSE_SHADING 0x0002
#define TEXTURE_MAPPED_SHADING 0x0004

typedef long Fixedpoint;
typedef unsigned int TAngle;  /* angle in tenths of degrees */
typedef Fixedpoint Xform[3][4];

/* Describes a single 2D point */
typedef struct _Point {
   int X;
   int Y;
} Point;

/* Describes a color in the current color model, the RGB color cube */
typedef struct _ModelColor {
   unsigned char Red;   /* 255 = max red, 0 = no red */
   unsigned char Green; /* 255 = max green, 0 = no green */
   unsigned char Blue;  /* 255 = max blue, 0 = no blue */
} ModelColor;

/* Describes an intensity in the current color model, the RGB color cube */
typedef struct _ModelIntensity {
   Fixedpoint Red;
   Fixedpoint Green;
   Fixedpoint Blue;
} ModelIntensity;

/* Describes a single 3D point in homogeneous coordinates; the W
   coordinate isn't present, though; assumed to be 1 and implied */
typedef struct _Point3 {
   Fixedpoint X;
   Fixedpoint Y;
   Fixedpoint Z;
} Point3;

typedef struct {
   int X;
   int Y;
   int Z;
} IntPoint3;

/* Describes a series of points (used to store a list of vertices that
   describe a polygon; each vertex is assumed to connect to the two
   adjacent vertices; last vertex is assumed to connect to first) */
typedef struct {
   int Length;
   Point * PointPtr;
} PointListHeader;

/* Describes the beginning and ending X coordinates of a single
   horizontal line */
typedef struct {
   int XStart;
   int XEnd;
} HLine;

/* Describes a Length-long series of horizontal lines, all assumed to
   be on contiguous scan lines starting at YStart and proceeding
   downward (used to describe a scan-converted polygon to the
   low-level hardware-dependent drawing code) */
typedef struct {
   int Length;
   int YStart;
   HLine * HLinePtr;
} HLineList;

/* Describes a rectangle */
typedef struct {
   int Left;
   int Top;
   int Right;
   int Bottom;
} Rect;

/* Describes a texture map */
typedef struct {
   int TexMapWidth;  /* texture map width in bytes */
   char *TexMapBits; /* pointer to texture bitmap */
} TextureMap;

/* Structure describing one face of an object (one polygon) */
typedef struct {
   int * VertNums;   /* pointer to list of indexes of this polygon's
                        vertices in the object's vertex list. The first two
                        indexes must select the end and start points,
                        respectively, of this polygon's unit normal vector.
                        The second point should also be an active polygon
                        vertex */
   int NumVerts;     /* # of verts in face, not including the initial
                        vertex, which must be the end of a unit normal vector
                        that starts at the second index in VertNums */
   int ColorIndex;   /* direct palette index; used only for non-shaded
                        faces */
   ModelColor FullColor; /* polygon's color */
   int ShadingType;  /* none, ambient, diffuse, texture mapped, etc. */
   TextureMap * TexMap; /* pointer to bitmap for texture mapping, if any */
   Point * TexVerts; /* pointer to list of this polygon's vertices, in
                        TextureMap coordinates. Index n must map to index
                        n + 1 in VertNums, (the + 1 is to skip over the unit
                        normal endpoint in VertNums) */
} Face;

typedef struct { TAngle RotateX, RotateY, RotateZ; } RotateControl;

/* Fields common to every object */
#define BASE_OBJECT                                              \
   struct _Object *NextObject;                                   \
   struct _Object *PreviousObject;                               \
   Point3 CenterInView;    /* coord of center in view space */   \
   void (*DrawFunc)();     /* draws object */                    \
   void (*RecalcFunc)();   /* prepares object for drawing */     \
   void (*MoveFunc)();     /* moves object */                    \
   int RecalcXform;        /* 1 to indicate need to recalc */    \
   Rect EraseRect[2];      /* rectangle to erase in each page */
/* Basic object */
typedef struct _Object { BASE_OBJECT } Object;
/* Structure describing a polygon-based object */
typedef struct {
   BASE_OBJECT
   int RDelayCount, RDelayCountBase; /* controls rotation speed */
   int MDelayCount, MDelayCountBase; /* controls movement speed */
   Xform XformToWorld;        /* transform from object->world space */
   Xform XformToView;         /* transform from object->view space */
   RotateControl Rotate;      /* controls rotation change over time */
   int NumVerts;              /* # vertices in VertexList */
   int NumRealVerts;          /* # vertices that aren't unit normal
                                 endpoints. Unit normal endpoints must appear
                                 last in VertexList, and aren't transformed
                                 into screen space or screen coordinates */
   Point3 * VertexList;       /* untransformed vertices */
   Point3 * XformedVertexList;   /* transformed into view space */
   Point3 * ProjectedVertexList; /* projected into screen space */
   Point * ScreenVertexList;     /* converted to screen coordinates */
   int NumFaces;              /* # of faces in object */
   Face * FaceList;           /* pointer to face info */
} PObject;

extern void XformVec(Xform, Fixedpoint *, Fixedpoint *);
extern void ConcatXforms(Xform, Xform, Xform);
extern int FillConvexPolygon(PointListHeader *, int, int, int);
extern void SetGraphicsMode(void);
extern void ShowPage(unsigned int);
extern void FillRectangleX(int, int, int, int, unsigned int, int);
extern void XformAndProjectPObject(PObject *);
extern void DrawPObject(PObject *);
extern void AppendRotationX(Xform, TAngle);
extern void AppendRotationY(Xform, TAngle);
extern void AppendRotationZ(Xform, TAngle);
extern Fixedpoint FixedMul(Fixedpoint, Fixedpoint);
extern Fixedpoint FixedDiv(Fixedpoint, Fixedpoint);
extern void InitializeFixedPoint(void);
extern void RotateAndMoveBall(PObject *);
extern void InitializeCubes(void);
extern void InitializeBalls(void);
extern void CosSin(TAngle, Fixedpoint *, Fixedpoint *);
extern void AddObject(Object *);
extern void SortObjects(void);
extern void InitializeObjectList(void);
extern int ModelColorToColorIndex(ModelColor * Color);
extern void IntensityAdjustColor(ModelColor *, ModelColor *,
   ModelIntensity *);
extern void InitializeLighting(void);
extern void SetAmbientIntensity(ModelIntensity *);
extern ModelIntensity * GetAmbientIntensity(void);
extern void TurnSpotOn(int);
extern void TurnSpotOff(int);
extern void SetSpotDirection(int, Point3 *);
extern void SetSpotIntensity(int, ModelIntensity *);
extern Point3 * GetSpotDirection(int);
extern ModelIntensity * GetSpotIntensity(int);
extern int GetSpotState(int);
extern void TurnAmbientOn(void);
extern void TurnAmbientOff(void);
extern int GetAmbientState(void);
extern void DrawTexturedPolygon(PointListHeader *, Point *, TextureMap *);
extern void WritePixelX(int, int, int);
extern int DisplayedPage, NonDisplayedPage, RecalcAllXforms;
extern int NumObjects;
extern Object ObjectListStart, ObjectListEnd;
extern Xform WorldViewXform;
extern Object *ObjectList[];
extern Point3 CubeVerts[];
extern unsigned int CurrentPageBase;
extern unsigned int PageStartOffsets[2];
extern int ClipMinX, ClipMinY, ClipMaxX, ClipMaxY;
extern ModelIntensity AmbientIntensity;
extern Point3 SpotDirectionWorld[];
extern Point3 SpotDirectionView[];
extern ModelIntensity SpotIntensity[];
extern int SpotOn[];
extern int AmbientOn;
extern int BallEvent;

