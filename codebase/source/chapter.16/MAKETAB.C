//  MAKETAB.C -- Build QSCAN3.INC for QSCAN3.ASM
 
#include <stdio.h>
#include <ctype.h>
 
#define ChType( c )  (((c) & 0x7f) == '\'' || isalnum((c) & 0x7f))
 
int NoCarry[ 4 ] = { 0, 0x80, 1, 0x80 };
int Carry[ 4 ]   = { 1, 0x81, 1, 0x80 };
 
void main( void )
  {
  int ahChar, alChar, i;
  FILE *t = fopen( "QSCAN3.INC", "wt" );
 
  printf( "Building table.  Please wait..." );
 
  for( ahChar = 0; ahChar < 128; ahChar++ )
    {
    for( alChar = 0; alChar < 256; alChar++ )
      {
      i = ChType( alChar ) * 2 + ChType( ahChar );
 
      if( alChar % 8 == 0 )  fprintf( t, "\ndb %02Xh", NoCarry[ i ] );
      else                   fprintf( t, ",%02Xh", NoCarry[ i ] );
 
      fprintf( t, ",%02Xh", Carry[ i ] );
      }
    }
 
  fclose( t );
  }

