/*
 * Program to search the file specified by the first command-line
 * argument for the string specified by the second command-line
 * argument. Performs the search by reading and searching blocks
 * of size BLOCK_SIZE.
 */
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <alloc.h>   /* alloc.h for Borland compilers,
                        malloc.h for Microsoft compilers */

#define BLOCK_SIZE  0x4000   /* we'll process the file in 16Kb
                                 blocks */

/* Searches the specified number of sequences in the specified buffer
   for matches to SearchString of SearchStringLength. Note that the
   calling code should already have shortened SearchLength if
   necessary to compensate for the distance from the end of the buffer
   to the last possible start of a matching sequence in the buffer */
int SearchForString(unsigned char *Buffer, int SearchLength,
   unsigned char *SearchString, int SearchStringLength)
{
   unsigned char *PotentialMatch;

   /* Search so long as there are potential-match locations
      remaining */
   while ( SearchLength ) {
      /* See if the first character of SearchString can be found */
      if ( (PotentialMatch =
            memchr(Buffer, *SearchString, SearchLength)) == NULL ) {
         break;   /* no matches in this buffer */
      }

      /* The first character matches; see if the rest of the string
         also matches */
      if ( SearchStringLength == 1 ) {
         return(1);  /* that 1 matching character was the whole
                        search string, so we've got a match */
      } else {
         /* Check whether the remaining characters match */
         if ( !memcmp(PotentialMatch + 1, SearchString + 1,
               SearchStringLength - 1) ) {
            return(1);  /* we've got a match */
         }
      }

      /* The string doesn't match; keep going by pointing past the
         potential-match location we just rejected */
      SearchLength -= PotentialMatch - Buffer + 1;
      Buffer = PotentialMatch + 1;
   }

   return(0);  /* no match found */
}

main(int argc, char *argv[]) {
   int Done;               /* indicates whether search is done */
   int Handle;             /* handle of file being searched */
   int WorkingLength;      /* length of current block */
   int SearchStringLength; /* length of string to search for */
   int BlockSearchLength;  /* length to search in current block */
   int Found;              /* indicates final search completion
                              status */
   int NextLoadCount;      /* # of bytes to read into next block,
                              accounting for bytes copied from the last
                              block */
   unsigned char *WorkingBlock;  /* block storage buffer */
   unsigned char *SearchString;  /* pointer to string to search for */
   unsigned char *NextLoadPtr;   /* offset at which to start loading
                                    the next block, accounting for
                                    bytes copied from the last
                                    block */

   /* Check for the proper number of arguments */
   if ( argc != 3 ) {
      printf("usage: search filename search-string\n");
      exit(1);
   }

   /* Try to open the file to be searched */
   if ( (Handle = open(argv[1], O_RDONLY | O_BINARY)) == -1 ) {
      printf("Can't open file: %s\n", argv[1]);
      exit(1);
   }

   /* Calculate the length of text to search for */
   SearchString = argv[2];
   SearchStringLength = strlen(SearchString);

   /* Try to get memory in which to buffer the data */
   if ( (WorkingBlock = malloc(BLOCK_SIZE)) == NULL ) {
      printf("Can't get enough memory\n");
      exit(1);
   }

   /* Load the first block at the start of the buffer, and try to fill
      the entire buffer */
   NextLoadPtr = WorkingBlock;
   NextLoadCount = BLOCK_SIZE;

   Done = 0;      /* not done with search yet */
   Found = 0;     /* assume we won't find a match */

   /* Search the file in BLOCK_SIZE chunks */
   do {
      /* Read in however many bytes are needed to fill out the block
         (accounting for bytes copied over from the last block), or
         the rest of the bytes in the file, whichever is less */
      if ( (WorkingLength = read(Handle, NextLoadPtr,
            NextLoadCount)) == -1 ) {
         printf("Error reading file %s\n", argv[1]);
         exit(1);
      }

      /* If we didn't read all the bytes we requested, we're done
         after this block whether we find a match or not */
      if ( WorkingLength != NextLoadCount ) {
         Done = 1;
      }

      /* Account for any bytes we copied from the end of the last
         block in the total length of this block */
      WorkingLength += NextLoadPtr - WorkingBlock;

      /* Calculate the number of bytes in this block that could
         possibly be the start of a matching sequence that lies
         entirely in this block (sequences that run off the end of the
         block will be transferred to the next block and found when
         that block is searched) */
      if ( (BlockSearchLength =
               WorkingLength - SearchStringLength + 1) <= 0 ) {
            Done = 1;   /* too few characters in this block for there
                        to be any possible matches, so this is the
                        final block and we're done without finding a
                        match */
      } else {
         /* Search this block */
         if ( SearchForString(WorkingBlock, BlockSearchLength,
               SearchString, SearchStringLength) ) {
            Found = 1;     /* we've found a match */
            Done = 1;
         } else {
            /* Copy any bytes from the end of the block that start
               potentially-matching sequences that would run off the
               end of the block over to the next block */
            if ( SearchStringLength > 1 ) {
               memcpy(WorkingBlock,
                  WorkingBlock + BLOCK_SIZE - SearchStringLength + 1,
                  SearchStringLength - 1);
            }

            /* Set up to load the next bytes from the file after the
               bytes copied from the end of the current block */
            NextLoadPtr = WorkingBlock + SearchStringLength - 1;
            NextLoadCount = BLOCK_SIZE - SearchStringLength + 1;
         }
      }
   } while ( !Done );

   /* Report the results */
   if ( Found ) {
      printf("String found\n");
   } else {
      printf("String not found\n");
   }

   exit(Found);   /* return the found/not found status as the
                     DOS errorlevel */
}

