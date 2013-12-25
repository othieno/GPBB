/* Searches a buffer for a specified pattern. In case of a mismatch,
   uses the value of the mismatched byte to skip across as many
   potential match locations as possible (partial Boyer-Moore).
   Returns start offset of first match searching forward, or NULL if
   no match is found.
   Tested with Borland C++ in C mode and the small model. */

#include <stdio.h>

unsigned char * FindString(unsigned char * BufferPtr,
   unsigned int BufferLength, unsigned char * PatternPtr,
   unsigned int PatternLength)
{
   unsigned char * WorkingPatternPtr, * WorkingBufferPtr;
   unsigned int CompCount, SkipTable[256], Skip, DistanceMatched;
   int i;

   /* Reject if the buffer is too small */
   if (BufferLength < PatternLength) return(NULL);

   /* Return an instant match if the pattern is 0-length */
   if (PatternLength == 0) return(BufferPtr);

   /* Create the table of distances by which to skip ahead on
      mismatches for every possible byte value */
   /* Initialize all skips to the pattern length; this is the skip
      distance for bytes that don't appear in the pattern */
   for (i = 0; i < 256; i++) SkipTable[i] = PatternLength;
   /* Set the skip values for the bytes that do appear in the pattern
      to the distance from the byte location to the end of the
      pattern. When there are multiple instances of the same byte, the
      rightmost instance's skip value is used. Note that the rightmost
      byte of the pattern isn't entered in the skip table; if we get
      that value for a mismatch, we know for sure that the right end
      of the pattern has already passed the mismatch location, so this
      is not a relevant byte for skipping purposes */
   for (i = 0; i < (PatternLength - 1); i++)
      SkipTable[PatternPtr[i]] = PatternLength - i - 1;

   /* Point to rightmost byte of the pattern */
   PatternPtr += PatternLength - 1;
   /* Point to last (rightmost) byte of the first potential pattern
      match location in the buffer */
   BufferPtr += PatternLength - 1;
   /* Count of number of potential pattern match locations in
      buffer */
   BufferLength -= PatternLength - 1;

   /* Search the buffer */
   while (1) {
      /* See if we have a match at this buffer location */
      WorkingPatternPtr = PatternPtr;
      WorkingBufferPtr = BufferPtr;
      CompCount = PatternLength;
      /* Compare the pattern and the buffer location, searching from
         high memory toward low (right to left) */
      while (*WorkingPatternPtr-- == *WorkingBufferPtr--) {
         /* If we've matched the entire pattern, it's a match */
         if (--CompCount == 0)
            /* Return a pointer to the start of the match location */
            return(BufferPtr - PatternLength + 1);
      }
      /* It's a mismatch; let's see what we can learn from it */
      WorkingBufferPtr++;  /* point back to the mismatch location */
      /* # of bytes that did match */
      DistanceMatched = BufferPtr - WorkingBufferPtr;
      /* If, based on the mismatch character, we can't even skip ahead
         as far as where we started this particular comparison, then
         just advance by 1 to the next potential match; otherwise,
         skip ahead from the mismatch location by the skip distance
         for the mismatch character */
      if (SkipTable[*WorkingBufferPtr] <= DistanceMatched)
         Skip = 1;   /* skip doesn't do any good, advance by 1 */
      else
         /* Use skip value, accounting for distance covered by the
            partial match */
         Skip = SkipTable[*WorkingBufferPtr] - DistanceMatched;
      /* If skipping ahead would exhaust the buffer, we're done
         without a match */
      if (Skip >= BufferLength) return(NULL);
      /* Skip ahead and perform the next comparison */
      BufferLength -= Skip;
      BufferPtr += Skip;
   }
}

