/* Calculates gamma corrected settings for n intensity selection bits given m
   palette bits. */

#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdlib.h>

#define GAMMA  2.3

void main() {
	double intensity,x;
	int i;
	int selbits, palbits;
	int sellevels, pallevels;

	printf("# of selection bits (eg., 3 if 8 intensity levels desired):");
	scanf("%d", &selbits);
	printf("# of palette bits (eg., 6 if 64 palette entries):");
	scanf("%d", &palbits);
   sellevels = 1;
   while (selbits--) { sellevels *= 2; };
   sellevels--;
   pallevels = 1;
   while (palbits--) { pallevels *= 2; };
   pallevels--;

	printf("0\n");
	for (i=1; i<=sellevels; i++) {
		intensity = (double)i/(double)sellevels;
		printf("%d\n", (int)((pow(intensity, 1.0/GAMMA)*pallevels) + 0.5));
	}
}
