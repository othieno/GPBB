/* Finds and returns the greatest common divisor of two integers.
   Uses Euclid's Algorithm: divides the larger integer by the
   smaller; if the remainder is 0, the smaller integer is the gcd,
   otherwise the smaller integer becomes the larger integer, the
   remainder becomes the smaller integer, and the process is
   repeated. Avoids code recursion.  */


unsigned int gcd(unsigned int int1, unsigned int int2) {
   unsigned int temp;

   /* Swap if necessary to make sure that int1 >= int2 */
   if (int1 < int2) {
      temp = int1;
      int1 = int2;
      int2 = temp;
   }

   /* Now loop, dividing int1 by int2 and checking the remainder,
      until the remainder is 0. At each step, if the remainder isn't
      0, assign int2 to int1, and the remainder to int2, then
      repeat */
   for (;;) {
      /* If the remainder of int1 divided by int2 is 0, then int2 is
         the gcd */
      if ((temp = int1 % int2) == 0) {
         return(int2);
      }
      /* Make int2 the larger integer and the remainder the
         smaller integer, and repeat the process */
      int1 = int2;
      int2 = temp;
   }
}

