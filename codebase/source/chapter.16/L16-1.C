/* Word-counting program.  Tested with Borland C++ in C
   compilation mode and the small model */

#include <stdio.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <stdlib.h>
#include <io.h>

#define BUFFER_SIZE  0x8000   /* largest chunk of file worked
                                 with at any one time */
int main(int, char **);

int main(int argc, char **argv) {
   int Handle;
   unsigned int BlockSize;
   long FileSize;
   unsigned long WordCount = 0;
   char *Buffer, CharFlag = 0, PredCharFlag, *BufferPtr, Ch;

   if (argc != 2) {
      printf("usage: wc <filename>\n");
      exit(1);
   }

   if ((Buffer = malloc(BUFFER_SIZE)) == NULL) {
      printf("Can't allocate adequate memory\n");
      exit(1);
   }

   if ((Handle = open(argv[1], O_RDONLY | O_BINARY)) == -1) {
      printf("Can't open file %s\n", argv[1]);
      exit(1);
   }

   if ((FileSize = filelength(Handle)) == -1) {
      printf("Error sizing file %s\n", argv[1]);
      exit(1);
   }

   /* Process the file in chunks */
   while (FileSize > 0) {
      /* Get the next chunk */
      FileSize -= (BlockSize = min(FileSize, BUFFER_SIZE));
      if (read(Handle, Buffer, BlockSize) == -1) {
         printf("Error reading file %s\n", argv[1]);
         exit(1);
      }
      /* Count words in the chunk */
      BufferPtr = Buffer;
      do {
         PredCharFlag = CharFlag;
         Ch = *BufferPtr++ & 0x7F; /* strip high bit, which some
                                      word processors set as an
                                      internal flag */
         CharFlag = ((Ch >= 'a') && (Ch <= 'z')) ||
                    ((Ch >= 'A') && (Ch <= 'Z')) ||
                    ((Ch >= '0') && (Ch <= '9')) ||
                    (Ch == '\'');
         if ((!CharFlag) && PredCharFlag) {
            WordCount++;
         }
      } while (--BlockSize);
   }

   /* Catch the last word, if any */
   if (CharFlag) {
      WordCount++;
   }
   printf("\nTotal words in file: %lu\n", WordCount);
   return(0);
}

