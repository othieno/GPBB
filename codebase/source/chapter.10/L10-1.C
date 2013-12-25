/* Finds and returns the greatest common divisor of two positive
   integers. Works by trying every integral divisor between the
   smaller of the two integers and 1, until a divisor that divides
   both integers evenly is found. All C code tested with Microsoft
   and Borland compilers */

unsigned int gcd(unsigned int int1, unsigned int int2) {
   unsigned int temp, trial_divisor;

   /* Swap if necessary to make sure that int1 >= int2 */
   if (int1 < int2) {
      temp = int1;
      int1 = int2;
      int2 = temp;
   }

   /* Now just try every divisor from int2 on down, until a common
      divisor is found. This can never be an infinite loop because
      1 divides everything evenly */
   for (trial_divisor = int2; ((int1 % trial_divisor) != 0) ||
         ((int2 % trial_divisor) != 0); trial_divisor--)
      ;
   return(trial_divisor);
}

