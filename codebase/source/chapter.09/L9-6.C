/* Sample use of Div function to perform division when the result
   doesn't fit in 16 bits */
      
#include <stdio.h>

extern unsigned int Div(unsigned int * Dividend,
		int DividendLength, unsigned int Divisor,
		unsigned int * Quotient);

main() {
   unsigned long m, i = 0x20000001;
   unsigned int k, j = 0x10;
  
   k = Div((unsigned int *)&i, sizeof(i), j, (unsigned int *)&m);
   printf("%lu / %u = %lu r %u\n", i, j, m, k);
}
