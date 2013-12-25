void far NextGen( void );

extern unsigned short CellMap[];
extern unsigned short far ChangeList1[];

#define LEFT        (-2)
#define RIGHT       (+2)
#define UP          (WIDTH * LEFT)
#define DOWN        (WIDTH * RIGHT)
#define UPPERLEFT   (UP + LEFT)
#define UPPERRIGHT  (UP + RIGHT)
#define LOWERLEFT   (DOWN + LEFT)
#define LOWERRIGHT  (DOWN + RIGHT)
#define WRAPLEFT    (RIGHT * (WIDTH - 1))
#define WRAPRIGHT   (LEFT  * (WIDTH - 1))
#define WRAPUP      (DOWN  * (HEIGHT - 1))
#define WRAPDOWN    (UP    * (HEIGHT - 1))