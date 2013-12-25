/* Generates table of cos(theta) for theta from 0 to pi/2, in
   specified degree increments. */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "polygon.h"

/* Value from the Borland C++ 3.0 math.h include */
#define PI  3.14159265358979323846

void main(void);

void main()
{
   double Angle = 0.0;
   int i, j, NumSteps;
   long Fixed;
   double Step;
   FILE *DestFile;

   printf("How many steps per degree? ");
   scanf("%d",&NumSteps);
   Step = PI/(180*NumSteps);

   if ((DestFile = fopen("costable.inc", "w")) == NULL) {
      printf("Error opening file; not created\n");
      exit(1);
   }

   fprintf(DestFile,"; 16.16 fixed-point cosines of angles from\n");
   fprintf(DestFile,"; pi/2, in steps of 1/10 degree.\n");
   for (i = 0; i < 90; i++) {
      fprintf(DestFile," dd ");
      for (j = 0; j < (NumSteps-1); j++) {
         Fixed = DOUBLE_TO_FIXED(cos(Angle));
         Angle = Angle + Step;
         fprintf(DestFile,"0%lxh,", Fixed);
      }
      Fixed = DOUBLE_TO_FIXED(cos(Angle));
      Angle = Angle + Step;
      fprintf(DestFile,"0%lxh\n", Fixed);
   }
   /* Finally, the pi/2 point */
   Fixed = DOUBLE_TO_FIXED(cos(Angle));
   fprintf(DestFile," dd 0%lxh\n", Fixed);

   exit(0);
}
