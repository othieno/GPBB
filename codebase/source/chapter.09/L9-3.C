/* Program to exercise buffer-search routines in Listings 9-1 & 9-2 */

#include <stdio.h>
#include <string.h>
     
#define DISPLAY_LENGTH  40

extern unsigned char * FindString(unsigned char *, unsigned int,
      unsigned char *, unsigned int, unsigned int);
void main(void);

static unsigned char TestBuffer[] = "When, in the course of human \
events, it becomes necessary for one people to dissolve the \
political bands which have connected them with another, and to \
assume among the powers of the earth the separate and equal station \
to which the laws of nature and of nature's God entitle them...";

void main() {
   static unsigned char TestString[] = "equal";
   unsigned char TempBuffer[DISPLAY_LENGTH+1];
   unsigned char *MatchPtr;

   /* Search for TestString and report the results */
   if ((MatchPtr = FindString(TestBuffer,
         (unsigned int) strlen(TestBuffer), TestString,
         (unsigned int) strlen(TestString), 1)) == NULL) {
      /* TestString wasn't found */
      printf("\"%s\" not found\n", TestString);
   } else {
      /* TestString was found. Zero-terminate TempBuffer; strncpy
         won't do it if DISPLAY_LENGTH characters are copied */
      TempBuffer[DISPLAY_LENGTH] = 0;
      printf("\"%s\" found. Next %d characters at match:\n\"%s\"\n",
            TestString, DISPLAY_LENGTH,
            strncpy(TempBuffer, MatchPtr, DISPLAY_LENGTH));
   }
}
