/* Finds and returns the greatest common divisor of two integers.
   Uses Euclid's Algorithm: divides the larger integer by the
   smaller; if the remainder is 0, the smaller integer is the gcd,
   otherwise the smaller integer becomes the larger integer, the
   remainder becomes the smaller integer, and the process is
   repeated.  */

static unsigned int gcd_recurs(unsigned int, unsigned int);

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

   /* Now call the recursive form of the function, which assumes
      that the first parameter is the larger of the two */
   return(gcd_recurs(int1, int2));
}

static unsigned int gcd_recurs(unsigned int larger_int,
      unsigned int smaller_int)
{
   int temp;

   /* If the remainder of larger_int divided by smaller_int is 0,
      then smaller_int is the gcd */
   if ((temp = larger_int % smaller_int) == 0) {
      return(smaller_int);
   }

   /* Make smaller_int the larger integer and the remainder the
      smaller integer, and call this function recursively to
      continue the process */
   return(gcd_recurs(smaller_int, temp));
}

