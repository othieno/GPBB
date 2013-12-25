// MAIN.C
//
// David Stafford

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <bios.h>
#include "life.h"

// functions in VIDEO.C
void enter_display_mode( void );
void exit_display_mode( void );
void show_text( int x, int y, char *text );


void InitCellmap( void )
  {
  unsigned int i, j, t, x, y, init;

  for( init = (HEIGHT * WIDTH * 3) / 2; init; init-- )
    {
    x = random( WIDTH * 3 );
    y = random( HEIGHT );

    CellMap[ (y * WIDTH) + x / 3 ] |= 0x1000 << (2 - (x % 3));
    }

  for( i = j = 0; i < WIDTH * HEIGHT; i++ )
    {
    if( CellMap[ i ] & 0x7000 )
      {
      ChangeList1[ j++ ] = (short)&CellMap[ i ];
      }
    }

  NextGen();   // Set cell states, prime the pump.
  }


void main( void )
  {
  unsigned long generation = 0;
  char gen_text[ 80 ];
  long start_time, end_time;
  unsigned int seed;

  printf( "Seed (0 for random seed): " );
  scanf( "%d", &seed );
  if( seed == 0 )  seed = (unsigned) time(NULL);
  srand( seed );

  #ifndef NODRAW
  enter_display_mode();
  show_text( 0, 10, "Generation:" );
  #endif

  InitCellmap();       // randomly initialize cell map

  _bios_timeofday( _TIME_GETCLOCK, &start_time );

  do
    {
    NextGen();
    generation++;

    #ifndef NOCOUNTER
    sprintf( gen_text, "%10lu", generation );
    show_text( 0, 12, gen_text );
    #endif
    }
  #ifdef GEN
  while( generation < GEN );
  #else
  while( !kbhit() );
  #endif

  _bios_timeofday( _TIME_GETCLOCK, &end_time );
  end_time -= start_time;

  #ifndef NODRAW
  getch();    // clear keypress
  exit_display_mode();
  #endif

  printf( "Total generations: %ld\nSeed: %u\n", generation, seed );
  printf( "%ld ticks\n", end_time );
  printf( "Time: %f generations/second\n",
          (double)generation / (double)end_time * 18.2 );
  }