/* Finds and returns the greatest common divisor of two positive
   integers. Works by subtracting the smaller integer from the
   larger integer until either the values match (in which case
   that's the gcd), or the larger integer becomes the smaller of
   the two, in which case the two integers swap roles and the
   subtraction process continues.  */

unsigned int gcd(unsigned int int1, unsigned int int2) {
   unsigned int temp;

   /* If the two integers are the same, that's the gcd and we're
      done */
   if (int1 == int2) {
      return(int1);
   }

   /* Swap if necessary to make sure that int1 >= int2 */
   if (int1 < int2) {
      temp = int1;
      int1 = int2;
      int2 = temp;
   }

   /* Subtract int2 from int1 until int1 is no longer the larger of
      the two */
   do {
      int1 -= int2;
   } while (int1 > int2);

   /* Now recursively call this function to continue the process */
   return(gcd(int1, int2));
}

