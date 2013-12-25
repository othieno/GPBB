/* dispset.c
    
   Win32 program to demonstrate EnumDisplaySettings() and
   ChangeDisplaySettings(), and to demonstrate using the
   normal mouse APIs for game input. Note that DirectInput
   provides better mouse support (movement granularity is
   finer, and one call per frame suffices for DirectInput),
   but is not yet available on all systems or for Windows
   NT.

   Program pops up a listbox of available video modes;
   the user selects one by double-clicking, then navigates
   around using the mouse for orientation and the keyboard
   for linear motion.

   Modified from zsort.c, a demonstration program for
   z-sorted hidden-surface removal; all zsort comments
   have been left in.

   Note: loss of focus (such as Alt-Tab) is not handled in
   this program; consequently, when you Alt-Tab away, the
   video mode and mouse speed remain as set by zsort. In a
   commercial-quality game, RestoreDefaultMode() and
   StopGameMouse() would be called on loss of focus, and
   SetVMode() and StartGameMouse() would be called when
   focus was reacquired.

   Note: the fact that the mouse clamps at the border of
   the screen is the motivation for multiple calls per
   frame to AccumulateGameMouseMove(); these calls need
   to be close enough together in time so that the mouse
   can never reach the edge of the screen. These calls
   should be spaced roughly the same time apart and should
   be kept to a reasonably low level (Quake does 6 calls
   per frame), because the calls themselves do take some
   time.

   Note: resolution is freely changeable on Win95 and NT,
   but bits-per-pixel is only changeable on the fly under
   NT, not Win95. This will cause fewer modes to be be
   listed on Win95 than on NT for a given video adapter.
*/

/*
   Win32 program to demonstrate z-sorted spans.

   Derived from the VC++ generic sample application.
   Tested with VC++ 2.0 running on Windows NT 3.5.

   Note: in this implementation, polygon faces must not be
   interpenetrating. Also, correct sorting is not guaranteed
   if two polygonal objects butt up against each other. In other
   words, each polygonal object must be made of a continuous,
   non-self-intersecting skin, and polygonal objects must not
   interpenetrate or touch in order for proper sorting to result.
   More complex, slower sorting is required to make those cases
   work reliably.
   
   Note: polygon processing could be considerably more efficient
   if polygons shared common edges and edges shared common vertices.
   Also, indirection to vertices could be used to avoid having to
   copy all the vertices during every clip test. Outcode-type
   testing could be used to determine completely clipped or
   unclipped polygons ahead of time, avoiding the need to clip and
   copy entirely for such polygons. Outcode-type tests work best in
   viewspace, with the frustum normalized so that the field of view
   is 90 degrees, so simple compares, rather than dot products, can
   be used to categorize points with respect to the frustum. See
   _Computer Graphics_, by Foley & van Dam, or _Procedural Elements
   of Computer Graphics_, by Rogers, for further information.
*/

#include <windows.h>   	// required for all Windows applications
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_POLY_VERTS      8       // assumes polygons have no more than
                                    //  four sides and are clipped a
                                    //  maximum of four times by frustum.
                                    //  Must be increased for more sides
                                    //  or more clip planes
#define MAX_SCREEN_HEIGHT   2048
#define MOVEMENT_SPEED      3.0
#define VMOVEMENT_SPEED     3.0
#define MAX_MOVEMENT_SPEED  30.0
#define PI                  3.141592
#define ROLL_SPEED          (PI/20.0)
#define PITCH_SPEED         (PI/20.0)
#define YAW_SPEED           (PI/20.0)
#define MAX_COORD           0x4000
#define NUM_FRUSTUM_PLANES  4
#define CLIP_PLANE_EPSILON  0.0001
#define MAX_SPANS           10000
#define MAX_SURFS           1000
#define MAX_EDGES           5000


typedef struct {
    double v[3];
} point_t;

typedef struct {
    double   x, y;
} point2D_t;

typedef struct {
    int x, y;
    int count;
    int color;
} span_t;

typedef struct {
    double  distance;
    point_t normal;
} plane_t;

typedef struct {
    int         color;
    int         numverts;
    point_t     verts[MAX_POLY_VERTS];
    plane_t     plane;
} polygon_t;

typedef struct surf_s {
    struct surf_s   *pnext, *pprev;
    int             color;
    int             visxstart;
    double          zinv00, zinvstepx, zinvstepy;
    int             state;
} surf_t;

typedef struct {
    int         numverts;
    point2D_t   verts[MAX_POLY_VERTS];
} polygon2D_t;

typedef struct convexobject_s {
    struct convexobject_s   *pnext;
    point_t                 center;
    int                     numpolys;
    polygon_t               *ppoly;
} convexobject_t;

typedef struct edge_s {
    int             x;
    int             xstep;
    int             leading;
    surf_t          *psurf;
    struct edge_s   *pnext, *pprev;
    struct edge_s   *pnextremove;
} edge_t;

BITMAPINFO *pbmiDIB;		// pointer to the BITMAPINFO
char *pDIB, *pDIBBase;		// pointers to DIB section we'll draw into
HBITMAP hDIBSection;        // handle of DIB section
HINSTANCE hInst;            // current instance
char szAppName[] = "Clip";  // The name of this application
char szTitle[]   = "3D clipping demo"; // The title bar text
HPALETTE hpalold, hpalDIB;
HWND hwndOutput, hwndLB;
int DIBWidth, DIBHeight;
int DIBPitch;
double  roll, pitch, yaw;
double  currentspeed;
point_t currentpos;
double  fieldofview, xcenter, ycenter;
double  xscreenscale, yscreenscale, maxscale;
double  maxscreenscaleinv;
int     numobjects;
double  speedscale = 1.0;
double  mousespeedscale = 0.01;
plane_t frustumplanes[NUM_FRUSTUM_PLANES];

double  mroll[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
double  mpitch[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
double  myaw[3][3] =  {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
point_t vpn, vright, vup;
point_t xaxis = {1, 0, 0};
point_t zaxis = {0, 0, 1};

polygon_t polys0[] = {
{15, 4, {{-10,10,-10}, {10,10,-10}, {10,-10,-10}, {-10,-10,-10}},
    {10, {0, 0, -1}}},
{14, 4, {{10,10,-10}, {10,10,10}, {10,-10,10}, {10,-10,-10}},
    {10, {1, 0, 0}}},
{13, 4, {{10,10,10}, {-10,10,10}, {-10,-10,10}, {10,-10,10}},
    {10, {0, 0, 1}}},
{12, 4, {{-10,10,10}, {-10,10,-10}, {-10,-10,-10}, {-10,-10,10}},
    {10, {-1, 0, 0}}},
{11, 4, {{-10,10,-10}, {-10,10,10}, {10,10,10}, {10,10,-10}},
    {10, {0, 1, 0}}},
{10, 4, {{-10,-10,-10}, {10,-10,-10}, {10,-10,10}, {-10,-10,10}},
    {10, {0, -1, 0}}},
};

polygon_t polys1[] = {
{1, 4, {{-200,0,-200}, {-200,0,200},
        {200,0,200}, {200,0,-200}}, {0, {0, 1, 0}}},
};

polygon_t polys2[] = {
{6, 4, {{0,10,0}, {20,10,0}, {10,10,-10}, {0,10,-10}},
    {10, {0, 1, 0}}},
{6, 4, {{-10,10,10}, {0,10,10}, {0,10,0}, {-10,10,0}},
    {10, {0, 1, 0}}},
{6, 4, {{0,10,0}, {0,10,-10}, {-10,10,-10}, {-10,10,0}},
    {10, {0, 1, 0}}},
{5, 4, {{0,-10,0}, {0,-10,-10}, {10,-10,-10}, {20,-10,0}},
    {10, {0, -1, 0}}},
{5, 4, {{-10,-10,10}, {-10,-10,0}, {0,-10,0}, {0,-10,10}},
    {10, {0, -1, 0}}},
{5, 4, {{-10,-10,0}, {-10,-10,-10}, {0,-10,-10}, {0,-10,0}},
    {10, {0, -1, 0}}},
{4, 4, {{-10,10,-10}, {10,10,-10}, {10,-10,-10}, {-10,-10,-10}},
    {10, {0, 0, -1}}},
{3, 4, {{10,10,-10}, {20,10,0}, {20,-10,0}, {10,-10,-10}},
    {14.14, {0.707, 0, -0.707}}},
{2, 4, {{20,10,0}, {0,10,0}, {0,-10,0}, {20,-10,0}},
    {0, {0, 0, 1}}},
{9, 4, {{0,10,0}, {0,10,10}, {0,-10,10}, {0,-10,0}},
    {0, {1, 0, 0}}},
{15, 4, {{0,10,10}, {-10,10,10}, {-10,-10,10}, {0,-10,10}},
    {10, {0, 0, 1}}},
{14, 4, {{-10,10,10}, {-10,10,-10}, {-10,-10,-10}, {-10,-10,10}},
    {10, {-1, 0, 0}}},
};

extern convexobject_t   objecthead;

convexobject_t objects[] = {
{&objects[1], {-50,0,70}, sizeof(polys2) / sizeof(polys2[0]), polys2},
{&objects[2], {0,20,70}, sizeof(polys0) / sizeof(polys0[0]), polys0},
{&objects[3], {50,0,70}, sizeof(polys0) / sizeof(polys0[0]), polys0},
{&objects[4], {-50,0,-70}, sizeof(polys2) / sizeof(polys2[0]), polys2},
{&objects[5], {0,20,-70}, sizeof(polys2) / sizeof(polys2[0]), polys2},
{&objects[6], {50,30,-70}, sizeof(polys0) / sizeof(polys0[0]), polys0},
{&objects[7], {-50,15,0}, sizeof(polys0) / sizeof(polys0[0]), polys0},
{&objects[8], {50,15,0}, sizeof(polys2) / sizeof(polys2[0]), polys2},
{&objects[9], {0,50,0}, sizeof(polys2) / sizeof(polys2[0]), polys2},
{&objects[10], {-100,100,115}, sizeof(polys2) / sizeof(polys2[0]), polys2},
{&objects[11], {-100,150,120}, sizeof(polys0) / sizeof(polys0[0]), polys0},
{&objects[12], {100,200,100}, sizeof(polys0) / sizeof(polys0[0]), polys0},
{&objects[13], {100,100,100}, sizeof(polys2) / sizeof(polys2[0]), polys2},
{&objecthead, {0,-20,0}, sizeof(polys1) / sizeof(polys1[0]), polys1},
};

// Head and tail for the object list
convexobject_t objecthead = {&objects[0]};

// Span, edge, and surface lists
span_t  spans[MAX_SPANS];
edge_t  edges[MAX_EDGES];
surf_t  surfs[MAX_SURFS];

// Bucket list of new edges to add on each scan line
edge_t  newedges[MAX_SCREEN_HEIGHT];

// Bucket list of edges to remove on each scan line
edge_t  *removeedges[MAX_SCREEN_HEIGHT];

// Head and tail for the active edge list
edge_t  edgehead;
edge_t  edgetail;

// Edge used as sentinel of new edge lists
edge_t  maxedge = {0x7FFFFFFF};

// Head/tail/sentinel/background surface of active surface stack
surf_t  surfstack;

// pointers to next available surface and edge
surf_t  *pavailsurf;
edge_t  *pavailedge;

int window_center_x, window_center_y;
int currentcolor;
int inLB, curLBIndex;

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int bpp;
    char description[20];
} VMODE;

#define MAXVMODES   100
VMODE vmodes[MAXVMODES];
int numvmodes;

VMODE lowresmodes[] = {
 {320, 200, 8}, {320, 200, 15}, {320, 200, 16}, {320, 200, 24}, {320, 200, 32},
 {320, 240, 8}, {320, 240, 15}, {320, 240, 16}, {320, 240, 24}, {320, 240, 32},
 {320, 350, 8}, {320, 350, 15}, {320, 350, 16}, {320, 350, 24}, {320, 350, 32},
 {320, 400, 8}, {320, 400, 15}, {320, 400, 16}, {320, 400, 24}, {320, 400, 32},
 {320, 480, 8}, {320, 480, 15}, {320, 480, 16}, {320, 480, 24}, {320, 480, 32},
 {360, 200, 8}, {360, 200, 15}, {360, 200, 16}, {360, 200, 24}, {360, 200, 32},
 {360, 240, 8}, {360, 240, 15}, {360, 240, 16}, {360, 240, 24}, {360, 240, 32},
 {360, 350, 8}, {360, 350, 15}, {360, 350, 16}, {360, 350, 24}, {360, 350, 32},
 {360, 400, 8}, {360, 400, 15}, {360, 400, 16}, {360, 400, 24}, {360, 400, 32},
 {360, 480, 8}, {360, 480, 15}, {360, 480, 16}, {360, 480, 24}, {360, 480, 32},
 {400, 300, 8}, {400, 300, 15}, {400, 300, 16}, {400, 300, 24}, {400, 300, 32},
 {512, 384, 8}, {512, 384, 15}, {512, 384, 16}, {512, 384, 24}, {512, 384, 32},
};

#define NUMLOWRESMODES (sizeof(lowresmodes)/sizeof(lowresmodes[0]))

BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int StartGameMouse (void);
void StopGameMouse (void);
void AccumulateGameMouseMove (void);
void GetGameMouseMoveForFrame (int * mouse_x_move, int * mouse_y_move);
void DoMouseMove(void);
void UpdateWorld(void);
int PutUpModeListBox(void);
void RestoreDefaultMode(void);
void SetVMode(int curLBIndex);

/////////////////////////////////////////////////////////////////////
// WinMain
/////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    HANDLE hAccelTable;

    if (!hPrevInstance) {       // Other instances of app running?
        if (!InitApplication(hInstance)) { // Initialize shared things
            return (FALSE);     // Exits if unable to initialize
        }
    }

    // Perform initializations that apply to a specific instance
    if (!InitInstance(hInstance, nCmdShow)) {
        return (FALSE);
    }

    if (!PutUpModeListBox())
        return (FALSE);

    hAccelTable = LoadAccelerators (hInstance, szAppName);

    // Acquire and dispatch messages until a WM_QUIT message is
    // received
    for (;;) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			do {
	    		if (msg.message == WM_QUIT) {
					goto Done;
				}
	      		if (!TranslateAccelerator (msg.hwnd, hAccelTable,
	      		        &msg)) {
            		TranslateMessage(&msg);// xlates virt keycodes
       	        	DispatchMessage(&msg); // Dispatches msg to window
	       	   	}
			} while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE));
		}

        if (!inLB)
        {
            // Do any mouse-controlled move for this frame
            DoMouseMove();

            // Update the world
		    UpdateWorld();
        }
	}

Done:
    return (msg.wParam); // Returns the value from PostQuitMessage

    lpCmdLine; // This will prevent 'unused formal parameter' warnings
}

/////////////////////////////////////////////////////////////////////
// InitApplication
/////////////////////////////////////////////////////////////////////
BOOL InitApplication(HINSTANCE hInstance)
{
        WNDCLASS  wc;

        // Fill in window class structure with parameters that
        // describe the main window.
        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc   = (WNDPROC)WndProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hInstance;
        wc.hIcon         = LoadIcon (hInstance, szAppName);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
        wc.lpszMenuName  = szAppName;
        wc.lpszClassName = szAppName;

        // Register the window class and return success/failure code.
        return (RegisterClass(&wc));
}


/////////////////////////////////////////////////////////////////////
// Now that we know the video mode, make the window the right size
// and create the DIB and set up the palette as needed.
/////////////////////////////////////////////////////////////////////
void SetUpToDraw(void)
{
	INT				i, j, k;
	LOGPALETTE *	plogpal;
	PUSHORT 		pusTemp;
	HPALETTE		hpal;
    HDC             hdc;

	DIBWidth = GetSystemMetrics(SM_CXSCREEN);
	DIBHeight = GetSystemMetrics(SM_CYSCREEN);
    window_center_x = DIBWidth / 2;
    window_center_y = DIBHeight / 2;

    SetWindowPos(hwndOutput,
                HWND_TOP,
                0,
                0,
                DIBWidth,
                DIBHeight,
                0);

    hdc = GetDC(hwndOutput);

    // For 256-color mode, set up the palette for maximum speed
    // in copying to the screen. If not a 256-color mode, the
    // adapter isn't palettized, so we'll just use the default
    // palette. However, we will run a lot slower in this case
    // due to translation while copying
    if (GetDeviceCaps(hdc, RASTERCAPS) & RC_PALETTE) {
        // This is a 256-color palettized mode.
    	// Set up and realize our palette and the identity color
    	// table for the DIB (identity means that DIB color
        // indices and screen palette indices match exactly, so
        // GDI doesn't have to do any translation when copying
        // from DIB to screen. This helps performance a lot)
		plogpal = malloc(sizeof(LOGPALETTE) +
	    	    256 * sizeof(PALETTEENTRY));

	    // Take up all physical palette entries, to flush out
	    // anything that's currently in the palette
	    plogpal->palVersion = 0x300;
		plogpal->palNumEntries = 236;
	    for (i=0; i<236; i++) {
		    plogpal->palPalEntry[i].peRed = i;
			plogpal->palPalEntry[i].peGreen = 0;
    		plogpal->palPalEntry[i].peBlue = 0;
	    	plogpal->palPalEntry[i].peFlags =
		    	    PC_RESERVED | PC_NOCOLLAPSE;
    	}

	    hpal = CreatePalette(plogpal);
		hpalold = SelectPalette(hdc, hpal, FALSE);
    	RealizePalette(hdc);
    	SelectPalette(hdc, hpalold, FALSE);
	    DeleteObject(hpal);

		// Now set up the 6value-6value-6value RGB palette,
		// followed by 20 gray levels, that we want to work with
		// into the physical palette
	    for (i=0; i<6; i++) {
			for (j=0; j<6; j++) {
	    		for (k=0; k<6; k++) {
			    	plogpal->palPalEntry[i*36+j*6+k].peRed =
                            i*255/6;
					plogpal->palPalEntry[i*36+j*6+k].peGreen =
					        j*255/6;
    				plogpal->palPalEntry[i*36+j*6+k].peBlue =
	   				        k*255/6;
	    			plogpal->palPalEntry[i*36+j*6+k].peFlags =
					    PC_RESERVED | PC_NOCOLLAPSE;
		    	}
    		}
	    }

		for (i=0; i<20; i++) {
    		plogpal->palPalEntry[i+216].peRed = i*255/20;
	    	plogpal->palPalEntry[i+216].peGreen = i*255/20;
			plogpal->palPalEntry[i+216].peBlue = i*255/20;
    		plogpal->palPalEntry[i+216].peFlags =
	    		    PC_RESERVED | PC_NOCOLLAPSE;
		}

    	hpal = CreatePalette(plogpal);
	    SelectPalette(hdc, hpal, FALSE);
	    RealizePalette(hdc);

		// Get back the 256 colors now in the physical palette,
		// which are the 236 we just selected, plus the 20 static
		// colors
    	GetSystemPaletteEntries(hdc, 0, 256, plogpal->palPalEntry);

    	for (i=10; i<246; i++) {
		    plogpal->palPalEntry[i].peFlags =
			        PC_RESERVED | PC_NOCOLLAPSE;
    	}

	    // Now create a logical palette that exactly matches the
		// physical palette, and realize it. This is the palette
    	// into which the DIB pixel values will be indices
    	plogpal->palNumEntries = 256;

	    hpalDIB = CreatePalette(plogpal);
	    SelectPalette(hdc, hpalDIB, FALSE);
	    DeleteObject(hpal);
    	RealizePalette(hdc);
        SelectPalette(hdc, hpalold, FALSE);
       	free(plogpal);
    }

	// Finally, set up the DIB section
	pbmiDIB = malloc(sizeof(BITMAPINFO) - 4 + 256*sizeof(USHORT));
	pbmiDIB->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmiDIB->bmiHeader.biWidth = DIBWidth;
	pbmiDIB->bmiHeader.biHeight = DIBHeight;
	pbmiDIB->bmiHeader.biPlanes = 1;
	pbmiDIB->bmiHeader.biBitCount = 8;
	pbmiDIB->bmiHeader.biCompression = BI_RGB;
	pbmiDIB->bmiHeader.biSizeImage = 0;
	pbmiDIB->bmiHeader.biXPelsPerMeter = 0;
	pbmiDIB->bmiHeader.biYPelsPerMeter = 0;
	pbmiDIB->bmiHeader.biClrUsed = 256;
	pbmiDIB->bmiHeader.biClrImportant = 256;

	pusTemp = (PUSHORT) pbmiDIB->bmiColors;

	for (i=0; i<256; i++) {
		*pusTemp++ = i;
	}

    hDIBSection = CreateDIBSection (hdc, pbmiDIB, DIB_PAL_COLORS,
                      &pDIBBase, NULL, 0);

    if (pbmiDIB->bmiHeader.biHeight > 0)
    {
        pDIB = (pDIBBase + (DIBHeight - 1) * DIBWidth);
        DIBPitch = -DIBWidth;   // bottom-up
    }
    else
    {
        pDIB = pDIBBase;
        DIBPitch = DIBWidth;    // top-down
    }

	// Clear the DIB
	memset(pDIBBase, 0, DIBWidth*DIBHeight);

	ReleaseDC(hwndOutput, hdc);

    // Set the initial location, direction, and speed
    roll = 0.0;
    pitch = 0.0;
    yaw = 0.0;
    currentspeed = 0.0;
    currentpos.v[0] = 0.0;
    currentpos.v[1] = 0.0;
    currentpos.v[2] = 0.0;
    fieldofview = 2.0;
    xscreenscale = DIBWidth / fieldofview;
    yscreenscale = DIBHeight / fieldofview;
    maxscale = max(xscreenscale, yscreenscale);
    maxscreenscaleinv = 1.0 / maxscale;
    xcenter = DIBWidth / 2.0 - 0.5;
    ycenter = DIBHeight / 2.0 - 0.5;

    numobjects = sizeof(objects) / sizeof(objects[0]);
}


/////////////////////////////////////////////////////////////////////
// InitInstance
/////////////////////////////////////////////////////////////////////
BOOL InitInstance(
        HINSTANCE       hInstance,
        int             nCmdShow)
{
        HWND            hwnd; // Main window handle.

        // Save the instance handle in static variable, which will be
        // used in many subsequent calls from this application to
        // Windows
        hInst = hInstance; // Store inst handle in our global variable

        // Create a main window for this application instance
	    DIBWidth = GetSystemMetrics(SM_CXSCREEN);
	    DIBHeight = GetSystemMetrics(SM_CYSCREEN);
        xcenter = DIBWidth / 2.0 - 0.5;
        ycenter = DIBHeight / 2.0 - 0.5;
   
        hwnd = CreateWindow(
                szAppName,           // See RegisterClass() call.
                szTitle,             // Text for window title bar.
                WS_POPUP,            // Window style.
                0,
                0,
				DIBWidth,
				DIBHeight,
                NULL,
                NULL,
                hInstance,
                NULL
        );

        // If window could not be created, return "failure"
        if (!hwnd) {
        	return (FALSE);
        }

		// Make the window visible and draw it
        ShowWindow(hwnd, nCmdShow); // Show the window
        UpdateWindow(hwnd);         // Sends WM_PAINT message

		hwndOutput = hwnd;

        return (TRUE);              // We succeeded...
}

/////////////////////////////////////////////////////////////////////
// WndProc
/////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(
                HWND hwnd,         // window handle
                UINT message,      // type of message
                WPARAM uParam,     // additional information
                LPARAM lParam)     // additional information
{
    int wmId, wmEvent;
	UINT fwSizeType;
	int oldDIBWidth, oldDIBHeight;
    HBITMAP holdDIBSection;
    HDC hdc;

    switch (message) {

    case WM_COMMAND:  // message: command from application menu

    	wmId    = LOWORD(uParam);
        wmEvent = HIWORD(uParam);

        switch (wmId) {
        case 1:
            if (wmEvent == LBN_DBLCLK)
            {
                curLBIndex = SendMessage(hwndLB, LB_GETCURSEL, 0, 0);
                DestroyWindow(hwndLB);
                SetVMode(curLBIndex);
                SetUpToDraw();
                StartGameMouse();
                inLB = 0;
            }
            break;

        default:
        	return (DefWindowProc(hwnd, message, uParam, lParam));
        }
        break;

	case WM_KEYDOWN:
		switch (uParam) {

     	case VK_ESCAPE:
        	DestroyWindow (hwnd);
            break;

        case VK_DOWN:
            currentspeed -= MOVEMENT_SPEED * speedscale;
            if (currentspeed < -(MAX_MOVEMENT_SPEED * speedscale))
                currentspeed = -(MAX_MOVEMENT_SPEED * speedscale);
			break;

        case VK_UP:
            currentspeed += MOVEMENT_SPEED * speedscale;
            if (currentspeed > (MAX_MOVEMENT_SPEED * speedscale))
                currentspeed = (MAX_MOVEMENT_SPEED * speedscale);
			break;

        case 'N':
            roll += ROLL_SPEED * speedscale;
            if (roll >= (PI * 2))
                roll -= PI * 2;
            break;

        case 'M':
            roll -= ROLL_SPEED * speedscale;
            if (roll < 0)
                roll += PI * 2;
            break;

        case 'A':
            pitch -= PITCH_SPEED * speedscale;
            if (pitch < 0)
                pitch += PI * 2;
            break;

        case 'Z':
            pitch += PITCH_SPEED * speedscale;
            if (pitch >= (PI * 2))
                pitch -= PI * 2;
            break;

        case 'D':
            currentpos.v[1] += VMOVEMENT_SPEED;
            break;

        case 'C':
            currentpos.v[1] -= VMOVEMENT_SPEED;
            break;

        case VK_LEFT:
            yaw -= YAW_SPEED * speedscale;
            if (yaw < 0)
                yaw += PI * 2;
			break;

        case VK_RIGHT:
            yaw += YAW_SPEED * speedscale;
            if (yaw >= (PI * 2))
                yaw -= PI * 2;
			break;

		default:
			break;
		}
		return(0);

	case WM_KEYUP:
		switch (uParam) {

		case VK_SUBTRACT:
            fieldofview *= 0.9;
            xscreenscale = DIBWidth / fieldofview;
            yscreenscale = DIBHeight / fieldofview;
            maxscale = max(xscreenscale, yscreenscale);
			break;

		case VK_ADD:
            fieldofview *= 1.1;
            xscreenscale = DIBWidth / fieldofview;
            yscreenscale = DIBHeight / fieldofview;
            maxscale = max(xscreenscale, yscreenscale);
			break;

        case 'F':
            speedscale *= 1.1;
            break;

        case 'S':
            speedscale *= 0.9;
            break;

		default:
			break;
		}
		return(0);

	case WM_SIZE:	// window size changed
		fwSizeType = uParam;
		if (fwSizeType != SIZE_MINIMIZED) {
            // Skip when this is called before the first DIB
            // section is created
            if (hDIBSection == 0)
                break;

			oldDIBWidth = DIBWidth;
			oldDIBHeight = DIBHeight;

			DIBWidth = LOWORD(lParam);
			DIBWidth = (DIBWidth + 3) & ~3;
			DIBHeight = HIWORD(lParam);

            if ((DIBHeight < 10) || (DIBWidth < 10))
            {
            // Keep the DIB section big enough so we don't start
            // drawing outside the DIB (the window can get smaller,
            // but we don't really care, and GDI will clip the
            // blts for us)
    			DIBWidth = oldDIBWidth;
    			DIBHeight = oldDIBHeight;
            }

			// Resize the DIB section to the new size
            holdDIBSection = hDIBSection;
			pbmiDIB->bmiHeader.biWidth = DIBWidth;
			pbmiDIB->bmiHeader.biHeight = DIBHeight;

            hdc = GetDC(hwnd);
            hDIBSection = CreateDIBSection (hdc, pbmiDIB,
                    DIB_PAL_COLORS, &pDIBBase, NULL, 0);

            if (hDIBSection) {
                // Success
                DeleteObject(holdDIBSection);

                if (pbmiDIB->bmiHeader.biHeight > 0)
                {
                    pDIB = (pDIBBase + (DIBHeight - 1) * DIBWidth);
                    DIBPitch = -DIBWidth;   // bottom-up
                }
                else
                {
                    pDIB = pDIBBase;
                    DIBPitch = DIBWidth;    // top-down
                }

                xscreenscale = DIBWidth / fieldofview;
                yscreenscale = DIBHeight / fieldofview;
                maxscale = max(xscreenscale, yscreenscale);
                xcenter = DIBWidth / 2.0 - 0.5;
                ycenter = DIBHeight / 2.0 - 0.5;
            } else {
                // Failed, just use old size
    			pbmiDIB->bmiHeader.biWidth = oldDIBWidth;
	    		pbmiDIB->bmiHeader.biHeight = oldDIBHeight;
    			DIBWidth = oldDIBWidth;
    			DIBHeight = oldDIBHeight;
            }

			// Clear the DIB
			memset(pDIBBase, 0, DIBWidth*DIBHeight);
		}
		break;

	case WM_DESTROY:  // message: window being destroyed
        // put back the default mouse and video mode settings
        StopGameMouse();
        RestoreDefaultMode();

		free(pbmiDIB);
        DeleteObject(hDIBSection);
		DeleteObject(hpalold);
                        
        PostQuitMessage(0);
        break;

    default:          // Passes it on if unproccessed
		return (DefWindowProc(hwnd, message, uParam, lParam));
    }
    return (0);
}

/////////////////////////////////////////////////////////////////////
// 3-D dot product.
/////////////////////////////////////////////////////////////////////
double DotProduct(point_t *vec1, point_t *vec2)
{
    return vec1->v[0] * vec2->v[0] +
           vec1->v[1] * vec2->v[1] +
           vec1->v[2] * vec2->v[2];
}

/////////////////////////////////////////////////////////////////////
// 3-D cross product.
/////////////////////////////////////////////////////////////////////
void CrossProduct(point_t *in1, point_t *in2, point_t *out)
{
    out->v[0] = in1->v[1] * in2->v[2] - in1->v[2] * in2->v[1];
    out->v[1] = in1->v[2] * in2->v[0] - in1->v[0] * in2->v[2];
    out->v[2] = in1->v[0] * in2->v[1] - in1->v[1] * in2->v[0];
}

/////////////////////////////////////////////////////////////////////
// Concatenate two 3x3 matrices.
/////////////////////////////////////////////////////////////////////
void MConcat(double in1[3][3], double in2[3][3], double out[3][3])
{
    int     i, j;

    for (i=0 ; i<3 ; i++)
    {
        for (j=0 ; j<3 ; j++)
        {
            out[i][j] = in1[i][0] * in2[0][j] +
                        in1[i][1] * in2[1][j] +
                        in1[i][2] * in2[2][j];
        }
    }
}

/////////////////////////////////////////////////////////////////////
// Project viewspace polygon vertices into screen coordinates.
// Note that the y axis goes up in worldspace and viewspace, but
// goes down in screenspace.
/////////////////////////////////////////////////////////////////////
void ProjectPolygon (polygon_t *ppoly, polygon2D_t *ppoly2D)
{
    int     i;
    double  zrecip;

    for (i=0 ; i<ppoly->numverts ; i++)
    {
        zrecip = 1.0 / ppoly->verts[i].v[2];
        ppoly2D->verts[i].x =
                ppoly->verts[i].v[0] * zrecip * maxscale + xcenter;
        ppoly2D->verts[i].y = ycenter -
                (ppoly->verts[i].v[1] * zrecip * maxscale);
    }

    ppoly2D->numverts = ppoly->numverts;
}

/////////////////////////////////////////////////////////////////////
// Move the view position and set the world->view transform.
/////////////////////////////////////////////////////////////////////
void UpdateViewPos()
{
    int     i;
    point_t motionvec;
    double  s, c, mtemp1[3][3], mtemp2[3][3];

    // Move in the view direction, across the x-y plane, as if
    // walking. This approach moves slower when looking up or
    // down at more of an angle
    motionvec.v[0] = DotProduct(&vpn, &xaxis);
    motionvec.v[1] = 0.0;
    motionvec.v[2] = DotProduct(&vpn, &zaxis);

    for (i=0 ; i<3 ; i++)
    {
        currentpos.v[i] += motionvec.v[i] * currentspeed;
        if (currentpos.v[i] > MAX_COORD)
            currentpos.v[i] = MAX_COORD;
        if (currentpos.v[i] < -MAX_COORD)
            currentpos.v[i] = -MAX_COORD;
    }

    // Set up the world-to-view rotation.
    // Note: much of the work done in concatenating these matrices
    // can be factored out, since it contributes nothing to the
    // final result; multiply the three matrices together on paper
    // to generate a minimum equation for each of the 9 final elements
    s = sin(roll);
    c = cos(roll);
    mroll[0][0] = c;
    mroll[0][1] = s;
    mroll[1][0] = -s;
    mroll[1][1] = c;

    s = sin(pitch);
    c = cos(pitch);
    mpitch[1][1] = c;
    mpitch[1][2] = s;
    mpitch[2][1] = -s;
    mpitch[2][2] = c;

    s = sin(yaw);
    c = cos(yaw);
    myaw[0][0] = c;
    myaw[0][2] = -s;
    myaw[2][0] = s;
    myaw[2][2] = c;

    MConcat(mroll, myaw, mtemp1);
    MConcat(mpitch, mtemp1, mtemp2);

    // Break out the rotation matrix into vright, vup, and vpn.
    // We could work directly with the matrix; breaking it out
    // into three vectors is just to make things clearer
    for (i=0 ; i<3 ; i++)
    {
        vright.v[i] = mtemp2[0][i];
        vup.v[i] = mtemp2[1][i];
        vpn.v[i] = mtemp2[2][i];
    }

    // Simulate crude friction
    if (currentspeed > (MOVEMENT_SPEED * speedscale / 2.0))
        currentspeed -= MOVEMENT_SPEED * speedscale / 2.0;
    else if (currentspeed < -(MOVEMENT_SPEED * speedscale / 2.0))
        currentspeed += MOVEMENT_SPEED * speedscale / 2.0;
    else
        currentspeed = 0.0;
}

/////////////////////////////////////////////////////////////////////
// Rotate a vector from viewspace to worldspace.
/////////////////////////////////////////////////////////////////////
void BackRotateVector(point_t *pin, point_t *pout)
{
    int     i;

    // Rotate into the world orientation
    for (i=0 ; i<3 ; i++)
    {
        pout->v[i] = pin->v[0] * vright.v[i] +
                     pin->v[1] * vup.v[i] +
                     pin->v[2] * vpn.v[i];
    }
}

/////////////////////////////////////////////////////////////////////
// Transform a point from worldspace to viewspace.
/////////////////////////////////////////////////////////////////////
void TransformPoint(point_t *pin, point_t *pout)
{
    int     i;
    point_t tvert;

    // Translate into a viewpoint-relative coordinate
    for (i=0 ; i<3 ; i++)
    {
        tvert.v[i] = pin->v[i] - currentpos.v[i];
    }

    // Rotate into the view orientation
    pout->v[0] = DotProduct(&tvert, &vright);
    pout->v[1] = DotProduct(&tvert, &vup);
    pout->v[2] = DotProduct(&tvert, &vpn);
}

/////////////////////////////////////////////////////////////////////
// Transform a polygon from worldspace to viewspace.
/////////////////////////////////////////////////////////////////////
void TransformPolygon(polygon_t *pinpoly, polygon_t *poutpoly)
{
    int     i;

    for (i=0 ; i<pinpoly->numverts ; i++)
    {
        TransformPoint(&pinpoly->verts[i], &poutpoly->verts[i]);
    }

    poutpoly->numverts = pinpoly->numverts;
}

/////////////////////////////////////////////////////////////////////
// Returns true if polygon faces the viewpoint, assuming a clockwise
// winding of vertices as seen from the front.
/////////////////////////////////////////////////////////////////////
int PolyFacesViewer(polygon_t *ppoly, plane_t *pplane)
{
    int     i;
    point_t viewvec;

    for (i=0 ; i<3 ; i++)
    {
        viewvec.v[i] = ppoly->verts[0].v[i] - currentpos.v[i];
    }

    // Use an epsilon here so we don't get polygons tilted so
    // sharply that the gradients are unusable or invalid
    if (DotProduct (&viewvec, &pplane->normal) < -0.01)
        return 1;
    else
        return 0;
}

/////////////////////////////////////////////////////////////////////
// Set up a clip plane with the specified normal.
/////////////////////////////////////////////////////////////////////
void SetWorldspaceClipPlane(point_t *normal, plane_t *plane)
{

    // Rotate the plane normal into worldspace
    BackRotateVector(normal, &plane->normal);

    plane->distance = DotProduct(&currentpos, &plane->normal) +
            CLIP_PLANE_EPSILON;
}

/////////////////////////////////////////////////////////////////////
// Set up the planes of the frustum, in worldspace coordinates.
/////////////////////////////////////////////////////////////////////
void SetUpFrustum(void)
{
    double  angle, s, c;
    point_t normal;

    angle = atan(2.0 / fieldofview * maxscale / xscreenscale);
    s = sin(angle);
    c = cos(angle);

    // Left clip plane
    normal.v[0] = s;
    normal.v[1] = 0;
    normal.v[2] = c;
    SetWorldspaceClipPlane(&normal, &frustumplanes[0]);

    // Right clip plane
    normal.v[0] = -s;
    SetWorldspaceClipPlane(&normal, &frustumplanes[1]);

    angle = atan(2.0 / fieldofview * maxscale / yscreenscale);
    s = sin(angle);
    c = cos(angle);

    // Bottom clip plane
    normal.v[0] = 0;
    normal.v[1] = s;
    normal.v[2] = c;
    SetWorldspaceClipPlane(&normal, &frustumplanes[2]);

    // Top clip plane
    normal.v[1] = -s;
    SetWorldspaceClipPlane(&normal, &frustumplanes[3]);
}

/////////////////////////////////////////////////////////////////////
// Clip a polygon to a plane.
/////////////////////////////////////////////////////////////////////
int ClipToPlane(polygon_t *pin, plane_t *pplane, polygon_t *pout)
{
    int     i, j, nextvert, curin, nextin;
    double  curdot, nextdot, scale;
    point_t *pinvert, *poutvert;

    pinvert = pin->verts;
    poutvert = pout->verts;

    curdot = DotProduct(pinvert, &pplane->normal);
    curin = (curdot >= pplane->distance);

    for (i=0 ; i<pin->numverts ; i++)
    {
        nextvert = (i + 1) % pin->numverts;

        // Keep the current vertex if it's inside the plane
        if (curin)
            *poutvert++ = *pinvert;

        nextdot = DotProduct(&pin->verts[nextvert], &pplane->normal);
        nextin = (nextdot >= pplane->distance);

        // Add a clipped vertex if one end of the current edge is
        // inside the plane and the other is outside
        if (curin != nextin)
        {
            scale = (pplane->distance - curdot) /
                    (nextdot - curdot);
            for (j=0 ; j<3 ; j++)
            {
                poutvert->v[j] = pinvert->v[j] +
                    ((pin->verts[nextvert].v[j] - pinvert->v[j]) *
                     scale);
            }
            poutvert++;
        }

        curdot = nextdot;
        curin = nextin;
        pinvert++;
    }

    pout->numverts = poutvert - pout->verts;
    if (pout->numverts < 3)
        return 0;

    return 1;
}

/////////////////////////////////////////////////////////////////////
// Clip a polygon to the frustum.
/////////////////////////////////////////////////////////////////////
int ClipToFrustum(polygon_t *pin, polygon_t *pout)
{
    int         i, curpoly;
    polygon_t   tpoly[2], *ppoly;

    curpoly = 0;
    ppoly = pin;

    for (i=0 ; i<(NUM_FRUSTUM_PLANES-1); i++)
    {
        if (!ClipToPlane(ppoly,
                         &frustumplanes[i],
                         &tpoly[curpoly]))
        {
            return 0;
        }
        ppoly = &tpoly[curpoly];
        curpoly ^= 1;
    }

    return ClipToPlane(ppoly,
                       &frustumplanes[NUM_FRUSTUM_PLANES-1],
                       pout);
}

/////////////////////////////////////////////////////////////////////
// Add the polygon's edges to the global edge table.
/////////////////////////////////////////////////////////////////////
void AddPolygonEdges (plane_t *plane, polygon2D_t *screenpoly)
{
    double  distinv, deltax, deltay, slope;
    int     i, nextvert, numverts, temp, topy, bottomy, height;
    edge_t  *pedge;

    numverts = screenpoly->numverts;

    // Clamp the polygon's vertices just in case some very near
    // points have wandered out of range due to floating-point
    // imprecision
    for (i=0 ; i<numverts ; i++)
    {
        if (screenpoly->verts[i].x < -0.5)
            screenpoly->verts[i].x = -0.5;
        if (screenpoly->verts[i].x > ((double)DIBWidth - 0.5))
            screenpoly->verts[i].x = (double)DIBWidth - 0.5;
        if (screenpoly->verts[i].y < -0.5)
            screenpoly->verts[i].y = -0.5;
        if (screenpoly->verts[i].y > ((double)DIBHeight - 0.5))
            screenpoly->verts[i].y = (double)DIBHeight - 0.5;
                }

    // Add each edge in turn
    for (i=0 ; i<numverts ; i++)
    {
        nextvert = i + 1;
        if (nextvert >= numverts)
            nextvert = 0;

        topy = (int)ceil(screenpoly->verts[i].y);
        bottomy = (int)ceil(screenpoly->verts[nextvert].y);
        height = bottomy - topy;
        if (height == 0)
            continue;       // doesn't cross any scan lines
        if (height < 0)
        {
            // Leading edge
            temp = topy;
            topy = bottomy;
            bottomy = temp;

            pavailedge->leading = 1;

            deltax = screenpoly->verts[i].x -
                     screenpoly->verts[nextvert].x;
            deltay = screenpoly->verts[i].y -
                     screenpoly->verts[nextvert].y;
            slope = deltax / deltay;

            // Edge coordinates are in 16.16 fixed point
            pavailedge->xstep = (int)(slope * (float)0x10000);
            pavailedge->x = (int)((screenpoly->verts[nextvert].x +
                ((float)topy - screenpoly->verts[nextvert].y) *
                slope) * (float)0x10000);
        }
        else
        {
            // Trailing edge
            pavailedge->leading = 0;

            deltax = screenpoly->verts[nextvert].x -
                     screenpoly->verts[i].x;
            deltay = screenpoly->verts[nextvert].y -
                     screenpoly->verts[i].y;
            slope = deltax / deltay;

            // Edge coordinates are in 16.16 fixed point
            pavailedge->xstep = (int)(slope * (float)0x10000);
            pavailedge->x = (int)((screenpoly->verts[i].x +
                ((float)topy - screenpoly->verts[i].y) * slope) *
                (float)0x10000);
        }

        // Put the edge on the list to be added on top scan
        pedge = &newedges[topy];
        while (pedge->pnext->x < pavailedge->x)
            pedge = pedge->pnext;
        pavailedge->pnext = pedge->pnext;
        pedge->pnext = pavailedge;

        // Put the edge on the list to be removed after final scan
        pavailedge->pnextremove = removeedges[bottomy - 1];
        removeedges[bottomy - 1] = pavailedge;

        // Associate the edge with the surface we'll create for
        // this polygon
        pavailedge->psurf = pavailsurf;

        // Make sure we don't overflow the edge array
        if (pavailedge < &edges[MAX_EDGES])
            pavailedge++;
    }

    // Create the surface, so we'll know how to sort and draw from
    // the edges
    pavailsurf->state = 0;
    pavailsurf->color = currentcolor;

    // Set up the 1/z gradients from the polygon, calculating the
    // base value at screen coordinate 0,0 so we can use screen
    // coordinates directly when calculating 1/z from the gradients
    distinv = 1.0 / plane->distance;
    pavailsurf->zinvstepx = plane->normal.v[0] * distinv *
            maxscreenscaleinv * (fieldofview / 2.0);
    pavailsurf->zinvstepy = -plane->normal.v[1] * distinv *
            maxscreenscaleinv * (fieldofview / 2.0);
    pavailsurf->zinv00 = plane->normal.v[2] * distinv -
            xcenter * pavailsurf->zinvstepx -
            ycenter * pavailsurf->zinvstepy;

    // Make sure we don't overflow the surface array
    if (pavailsurf < &surfs[MAX_SURFS])
        pavailsurf++;
}

/////////////////////////////////////////////////////////////////////
// Scan all the edges in the global edge table into spans.
/////////////////////////////////////////////////////////////////////
void ScanEdges (void)
{
    int     x, y;
    double  fx, fy, zinv, zinv2;
    edge_t  *pedge, *pedge2, *ptemp;
    span_t  *pspan;
    surf_t  *psurf, *psurf2;

    pspan = spans;

    // Set up the active edge list as initially empty, containing
    // only the sentinels (which are also the background fill). Most
    // of these fields could be set up just once at start-up
    edgehead.pnext = &edgetail;
    edgehead.pprev = NULL;
    edgehead.x = -0xFFFF;           // left edge of screen
    edgehead.leading = 1;
    edgehead.psurf = &surfstack;

    edgetail.pnext = NULL;          // mark edge of list
    edgetail.pprev = &edgehead;
    edgetail.x = DIBWidth << 16;    // right edge of screen
    edgetail.leading = 0;
    edgetail.psurf = &surfstack;

    // The background surface is the entire stack initially, and
    // is infinitely far away, so everything sorts in front of it.
    // This could be set just once at start-up
    surfstack.pnext = surfstack.pprev = &surfstack;
    surfstack.color = 0;
    surfstack.zinv00 = -999999.0;
    surfstack.zinvstepx = surfstack.zinvstepy = 0.0;

    for (y=0 ; y<DIBHeight ; y++)
    {
        fy = (double)y;

        // Sort in any edges that start on this scan
        pedge = newedges[y].pnext;
        pedge2 = &edgehead;
        while (pedge != &maxedge)
        {
            while (pedge->x > pedge2->pnext->x)
                pedge2 = pedge2->pnext;

            ptemp = pedge->pnext;
            pedge->pnext = pedge2->pnext;
            pedge->pprev = pedge2;
            pedge2->pnext->pprev = pedge;
            pedge2->pnext = pedge;

            pedge2 = pedge;
            pedge = ptemp;
        }

        // Scan out the active edges into spans

        // Start out with the left background edge already inserted,
        // and the surface stack containing only the background
        surfstack.state = 1;
        surfstack.visxstart = 0;

        for (pedge=edgehead.pnext ; pedge ; pedge=pedge->pnext)
        {
            psurf = pedge->psurf;

            if (pedge->leading)
            {
                // It's a leading edge. Figure out where it is
                // relative to the current surfaces and insert in
                // the surface stack; if it's on top, emit the span
                // for the current top.
                // First, make sure the edges don't cross
                if (++psurf->state == 1)
                {
                    fx = (double)pedge->x * (1.0 / (double)0x10000);
                    // Calculate the surface's 1/z value at this pixel
                    zinv = psurf->zinv00 + psurf->zinvstepx * fx +
                            psurf->zinvstepy * fy;

                    // See if that makes it a new top surface
                    psurf2 = surfstack.pnext;
                    zinv2 = psurf2->zinv00 + psurf2->zinvstepx * fx +
                            psurf2->zinvstepy * fy;
                    if (zinv >= zinv2)
                    {
                        // It's a new top surface
                        // emit the span for the current top
                        x = (pedge->x + 0xFFFF) >> 16;
                        pspan->count = x - psurf2->visxstart;
                        if (pspan->count > 0)
                        {
                            pspan->y = y;
                            pspan->x = psurf2->visxstart;
                            pspan->color = psurf2->color;

                            // Make sure we don't overflow
                            // the span array
                            if (pspan < &spans[MAX_SPANS])
                                pspan++;
                        }

                        psurf->visxstart = x;

                        // Add the edge to the stack
                        psurf->pnext = psurf2;
                        psurf2->pprev = psurf;
                        surfstack.pnext = psurf;
                        psurf->pprev = &surfstack;
                    }
                    else
                    {
                        // Not a new top; sort into the surface stack.
                        // Guaranteed to terminate due to sentinel
                        // background surface
                        do
                        {
                            psurf2 = psurf2->pnext;
                            zinv2 = psurf2->zinv00 +
                                    psurf2->zinvstepx * fx +
                                    psurf2->zinvstepy * fy;
                        } while (zinv < zinv2);

                        // Insert the surface into the stack
                        psurf->pnext = psurf2;
                        psurf->pprev = psurf2->pprev;
                        psurf2->pprev->pnext = psurf;
                        psurf2->pprev = psurf;
                    }
                }
            }
            else
            {
                // It's a trailing edge; if this was the top surface,
                // emit the span and remove it.
                // First, make sure the edges didn't cross
                if (--psurf->state == 0)
                {
                    if (surfstack.pnext == psurf)
                    {
                        // It's on top, emit the span
                        x = ((pedge->x + 0xFFFF) >> 16);
                        pspan->count = x - psurf->visxstart;
                        if (pspan->count > 0)
                        {
                            pspan->y = y;
                            pspan->x = psurf->visxstart;
                            pspan->color = psurf->color;

                            // Make sure we don't overflow
                            // the span array
                            if (pspan < &spans[MAX_SPANS])
                                pspan++;
                        }

                        psurf->pnext->visxstart = x;
                    }

                    // Remove the surface from the stack
                    psurf->pnext->pprev = psurf->pprev;
                    psurf->pprev->pnext = psurf->pnext;
                }
            }
        }

        // Remove edges that are done
        pedge = removeedges[y];
        while (pedge)
        {
            pedge->pprev->pnext = pedge->pnext;
            pedge->pnext->pprev = pedge->pprev;
            pedge = pedge->pnextremove;
        }

        // Step the remaining edges one scan line, and re-sort
        for (pedge=edgehead.pnext ; pedge != &edgetail ; )
        {
            ptemp = pedge->pnext;

            // Step the edge
            pedge->x += pedge->xstep;

            // Move the edge back to the proper sorted location,
            // if necessary
            while (pedge->x < pedge->pprev->x)
            {
                pedge2 = pedge->pprev;
                pedge2->pnext = pedge->pnext;
                pedge->pnext->pprev = pedge2;
                pedge2->pprev->pnext = pedge;
                pedge->pprev = pedge2->pprev;
                pedge->pnext = pedge2;
                pedge2->pprev = pedge;
            }

            pedge = ptemp;
        }
    }

    pspan->x = -1;  // mark the end of the list
}

/////////////////////////////////////////////////////////////////////
// Draw all the spans that were scanned out.
/////////////////////////////////////////////////////////////////////
void DrawSpans (void)
{
    span_t  *pspan;

    for (pspan=spans ; pspan->x != -1 ; pspan++)
    {
        memset (pDIB + (DIBPitch * pspan->y) + pspan->x,
                pspan->color,
                pspan->count);
    }
}

/////////////////////////////////////////////////////////////////////
// Clear the lists of edges to add and remove on each scan line.
/////////////////////////////////////////////////////////////////////
void ClearEdgeLists(void)
{
    int i;

    for (i=0 ; i<DIBHeight ; i++)
    {
        newedges[i].pnext = &maxedge;
        removeedges[i] = NULL;
    }
}

/////////////////////////////////////////////////////////////////////
// Render the current state of the world to the screen.
/////////////////////////////////////////////////////////////////////
void UpdateWorld()
{
	HPALETTE        holdpal;
    HDC             hdcScreen, hdcDIBSection;
    HBITMAP         holdbitmap;
    polygon2D_t     screenpoly;
    polygon_t       *ppoly, tpoly0, tpoly1, tpoly2;
    convexobject_t  *pobject;
    int             i, j, k;
    plane_t         plane;
    point_t         tnormal;

    UpdateViewPos();
    SetUpFrustum();
    ClearEdgeLists();
    pavailsurf = surfs;
    pavailedge = edges;


    // Draw all visible faces in all objects
    pobject = objecthead.pnext;

    // Accumulate the latest mouse movement and recenter the mouse before
    // building the edge list. In a real game, this would need to be done
    // about 50 times a second total, in as close to equal intervals as
    // possible
    AccumulateGameMouseMove();

    while (pobject != &objecthead)
    {
        ppoly = pobject->ppoly;

        for (i=0 ; i<pobject->numpolys ; i++)
        {
            // Move the polygon relative to the object center
            tpoly0.numverts = ppoly[i].numverts;
            for (j=0 ; j<tpoly0.numverts ; j++)
            {
                for (k=0 ; k<3 ; k++)
                    tpoly0.verts[j].v[k] = ppoly[i].verts[j].v[k] +
                            pobject->center.v[k];
            }

            if (PolyFacesViewer(&tpoly0, &ppoly[i].plane))
            {
                if (ClipToFrustum(&tpoly0, &tpoly1))
                {
                    currentcolor = ppoly[i].color;
                    TransformPolygon (&tpoly1, &tpoly2);
                    ProjectPolygon (&tpoly2, &screenpoly);

                    // Move the polygon's plane into viewspace
                    // First move it into worldspace (object relative)
                    tnormal = ppoly[i].plane.normal;
                    plane.distance = ppoly[i].plane.distance +
                        DotProduct (&pobject->center, &tnormal);
                    // Now transform it into viewspace
                    // Determine the distance from the viewpont
                    plane.distance -=
                          DotProduct (&currentpos, &tnormal);
                    // Rotate the normal into view orientation
                    plane.normal.v[0] =
                            DotProduct (&tnormal, &vright);
                    plane.normal.v[1] =
                            DotProduct (&tnormal, &vup);
                    plane.normal.v[2] =
                            DotProduct (&tnormal, &vpn);

                    AddPolygonEdges (&plane, &screenpoly);
                }
            }
        }

        pobject = pobject->pnext;
    }

    // Accumulate the latest mouse movement and recenter the mouse before
    // scanning the edges. In a real game, this would need to be done
    // about 50 times a second total, in as close to equal intervals as
    // possible
    AccumulateGameMouseMove();

    ScanEdges ();

    // Accumulate the latest mouse movement and recenter the mouse before
    // drawing the spans. In a real game, this would need to be done about
    // 50 times a second total, in as close to equal intervals as possible
    AccumulateGameMouseMove();

    DrawSpans ();

    // Accumulate the latest mouse movement and recenter the mouse before
    // bltting to the screen. In a real game, this would need to be done
    // about 50 times a second total, in as close to equal intervals as
    // possible
    AccumulateGameMouseMove();

	// We've drawn the frame; copy it to the screen
	hdcScreen = GetDC(hwndOutput);
	holdpal = SelectPalette(hdcScreen, hpalDIB, FALSE);
	RealizePalette(hdcScreen);

    hdcDIBSection = CreateCompatibleDC(hdcScreen);
    holdbitmap = SelectObject(hdcDIBSection, hDIBSection);

    BitBlt(hdcScreen, 0, 0, DIBWidth, DIBHeight, hdcDIBSection,
           0, 0, SRCCOPY);

	SelectPalette(hdcScreen, holdpal, FALSE);
	ReleaseDC(hwndOutput, hdcScreen);
    SelectObject(hdcDIBSection, holdbitmap);
    DeleteDC(hdcDIBSection);
}


/////////////////////////////////////////////////////////////////////
// Do mouse move.
/////////////////////////////////////////////////////////////////////
void DoMouseMove(void)
{
    int mouse_x_move, mouse_y_move;

    GetGameMouseMoveForFrame (&mouse_x_move, &mouse_y_move);

    yaw += YAW_SPEED * mousespeedscale * mouse_x_move;

    if (yaw < 0)
        yaw += PI * 2;
    else if (yaw >= (PI * 2))
        yaw -= PI * 2;

    pitch += PITCH_SPEED * mousespeedscale * mouse_y_move;

    if (pitch < 0)
        pitch += PI * 2;
    else if (pitch >= (PI * 2))
        pitch -= PI * 2;
}


/////////////////////////////////////////////////////////////////////
// functions to start up and shutdown the game configuration for
// the mouse, and to accumulate mouse moves over multiple calls
// during a frame and to calculate the total move for the frame
/////////////////////////////////////////////////////////////////////

// the two 0 values disable the two acceleration thresholds, and the
// 1 value specifies medium mouse speed
static int     originalmouseparms[3], newmouseparms[3] = {0, 0, 1};
static int     mouse_x_accum, mouse_y_accum;
static POINT   current_pos;

int StartGameMouse (void)
{
   if (!SystemParametersInfo (SPI_GETMOUSE, 0, originalmouseparms, 0))
      return 0;

   if (!SystemParametersInfo (SPI_SETMOUSE, 0, newmouseparms, 0))
      return 0;

   ShowCursor (FALSE);
   SetCursorPos (window_center_x, window_center_y);
   return 1;
}

void StopGameMouse (void)
{
   SystemParametersInfo (SPI_SETMOUSE, 0, originalmouseparms, 0);
   ShowCursor (TRUE);
}

void AccumulateGameMouseMove (void)
{
   GetCursorPos (&current_pos);

   mouse_x_accum += current_pos.x - window_center_x;
   mouse_y_accum += current_pos.y - window_center_y;

   // force the mouse to the center, so there's room to move
   SetCursorPos (window_center_x, window_center_y);
}

void GetGameMouseMoveForFrame (int * mouse_x_move, int * mouse_y_move)
{
   GetCursorPos (&current_pos);
   *mouse_x_move = current_pos.x - window_center_x + mouse_x_accum;
   *mouse_y_move = current_pos.y - window_center_y + mouse_y_accum;
   mouse_x_accum = 0;
   mouse_y_accum = 0;

   // force the mouse to the center, so there's room to move
   SetCursorPos (window_center_x, window_center_y);
}


/////////////////////////////////////////////////////////////////////
// Adds a video mode to the list if it's not a duplicate and can
// actually be set.
/////////////////////////////////////////////////////////////////////
void TryToAddToList(DEVMODE *pdevmode)
{
    int     i;

    // see if this is a duplicate mode (can happen because of refresh
    // rates, or because we explicitly try all the low-res modes)
    for (i=0; i<numvmodes; i++)
    {
        if ((pdevmode->dmBitsPerPel == vmodes[i].bpp) &&
            (pdevmode->dmPelsWidth == vmodes[i].width) &&
            (pdevmode->dmPelsHeight == vmodes[i].height))
        {
            // it's a duplicate mode
            return;
        }
    }

    // do a mode set test (doesn't actually do the mode set, but
    // reports whether it would have succeeded)
    if (ChangeDisplaySettings(pdevmode, CDS_TEST | CDS_FULLSCREEN) !=
        DISP_CHANGE_SUCCESSFUL)
    {
        return;
    }

    // filter outrageous values so the sprintf below is safe
    if ((pdevmode->dmBitsPerPel > 32) ||
        (pdevmode->dmPelsWidth > 9999) ||
        (pdevmode->dmPelsHeight > 9999))
    {
        return;
    }

    // it's a new, valid mode; add this to the list
    vmodes[numvmodes].bpp = pdevmode->dmBitsPerPel;
    vmodes[numvmodes].width = pdevmode->dmPelsWidth;
    vmodes[numvmodes].height = pdevmode->dmPelsHeight;
    sprintf (vmodes[numvmodes].description, "%dx%d, %d bpp",
             pdevmode->dmPelsWidth, pdevmode->dmPelsHeight,
             pdevmode->dmBitsPerPel);
    numvmodes++;
}


/////////////////////////////////////////////////////////////////////
// Puts up video mode list box.
/////////////////////////////////////////////////////////////////////
int PutUpModeListBox(void)
{
    int i, modenum, done;
    DEVMODE devmode;


    // enumerate all available video modes
    numvmodes = 0;
    modenum = 0;
    done = 0;

    do
    {
        done = !EnumDisplaySettings(NULL, modenum, &devmode);
        TryToAddToList(&devmode);
        modenum++;
    } while (!done & (numvmodes < MAXVMODES));

    // low-res modes don't always enumerate; ask about them explicitly

    // make sure the driver doesn't just answer yes to all tests
    devmode.dmBitsPerPel = 8;
    devmode.dmPelsWidth = 42;
    devmode.dmPelsHeight = 37;
    devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    if (ChangeDisplaySettings(&devmode, CDS_TEST | CDS_FULLSCREEN) !=
        DISP_CHANGE_SUCCESSFUL)
    {
        for (i=0; (i<NUMLOWRESMODES) && (numvmodes < MAXVMODES); i++)
        {
            devmode.dmSize = sizeof(devmode);
            devmode.dmBitsPerPel = lowresmodes[i].bpp;
            devmode.dmPelsWidth = lowresmodes[i].width;
            devmode.dmPelsHeight = lowresmodes[i].height;
            devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
            TryToAddToList(&devmode);
        }
    }

    // let user select desired mode from listbox
    hwndLB = CreateWindow("listbox",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | LBS_NOTIFY,
        DIBWidth / 2 - 150,
        DIBHeight / 2 - 200,
        300,
        400,
        hwndOutput,
        (HANDLE)1,
        hInst,
        NULL);

    if (!hwndLB)
        return 0;

    SetFocus(hwndLB);

    for (i=0; i<numvmodes; i++)
    {
        SendMessage(hwndLB, LB_ADDSTRING, 0, (LONG)(LPSTR)vmodes[i].description);
    }

    inLB = 1;

    return 1;
}


/////////////////////////////////////////////////////////////////////
// Restores default desktop video mode.
/////////////////////////////////////////////////////////////////////
void RestoreDefaultMode(void)
{
    ChangeDisplaySettings(NULL, CDS_FULLSCREEN);
}


/////////////////////////////////////////////////////////////////////
// Sets the mode vmodes[curLBIndex].
/////////////////////////////////////////////////////////////////////
void SetVMode(int curLBIndex)
{
    DEVMODE devmode;

    devmode.dmSize = sizeof(devmode);
    devmode.dmBitsPerPel = vmodes[curLBIndex].bpp;
    devmode.dmPelsWidth = vmodes[curLBIndex].width;
    devmode.dmPelsHeight = vmodes[curLBIndex].height;
    devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    if (ChangeDisplaySettings(&devmode, CDS_FULLSCREEN) !=
        DISP_CHANGE_SUCCESSFUL)
    {
        // mode set failed; we'll just run in the desktop mode
    }
}

