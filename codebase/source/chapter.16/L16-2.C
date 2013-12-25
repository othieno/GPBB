/* Word-counting program incorporating assembly language. Tested
	with Borland C++ in C compilation mode & the small model */

#include <stdio.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <stdlib.h>
#include <io.h>

#define BUFFER_SIZE  0x8000   /* largest chunk of file worked
                                 with at any one time */
int main(int, char **);
void ScanBuffer(char *, unsigned int, char *, unsigned long *);

int main(int argc, char **argv) {
   int Handle;
   unsigned int BlockSize;
   long FileSize;
   unsigned long WordCount = 0;
   char *Buffer, CharFlag = 0;

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

   CharFlag = 0;
   while (FileSize > 0) {
      FileSize -= (BlockSize = min(FileSize, BUFFER_SIZE));
      if (read(Handle, Buffer, BlockSize) == -1) {
         printf("Error reading file %s\n", argv[1]);
         exit(1);
      }
      ScanBuffer(Buffer, BlockSize, &CharFlag, &WordCount);
   }

   /* Catch the last word, if any */
   if (CharFlag) {
      WordCount++;
   }
   printf("\nTotal words in file: %lu\n", WordCount);
   return(0);
}

