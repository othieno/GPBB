/* Program to search through an array spanning a linked list of
   variable-sized blocks for all entries with a specified ID number
   and return the average of the values of all such entries. Each of
   the variable-sized blocks may contain any number of data entries,
   stored in the form of two separate arrays, one for ID numbers and
   one for values.
*/         

#include <stdio.h>
#ifdef __TURBOC__
#include <alloc.h>
#else
#include <malloc.h>
#endif

void main(void);
void exit(int);
extern unsigned int FindIDAverage2(unsigned int,struct BlockHeader *);

/* Structure that starts each variable-sized block */
struct BlockHeader {
   struct BlockHeader *NextBlock; /* pointer to next block, or NULL
                                    if this is the last block in the
                                    linked list */
   unsigned int BlockCount;      /* the number of DataElement entries
                                    in this variable-sized block */
};

void main(void) {
   int i,j;
   unsigned int IDToFind;
   struct BlockHeader *BaseArrayBlockPointer,*WorkingBlockPointer;
   int *WorkingDataPointer;
   struct BlockHeader **LastBlockPointer;

   printf("ID # for which to find average: ");
   scanf("%d",&IDToFind);

   /* Build an array across 5 blocks, for testing */
   /* Anchor the linked list to BaseArrayBlockPointer */
   LastBlockPointer = &BaseArrayBlockPointer;
   /* Create 5 blocks of varying sizes */
   for (i = 1; i < 6; i++) {
      /* Try to get memory for the next block */
      if ((WorkingBlockPointer =
            (struct BlockHeader *) malloc(sizeof(struct BlockHeader) +
            sizeof(int) * 2 * i * 10)) == NULL) {
         exit(1);
      }
      /* Set the # of data elements in this block */
      WorkingBlockPointer->BlockCount = i * 10;
      /* Link the new block into the chain */
      *LastBlockPointer = WorkingBlockPointer;
      /* Point to the first data field */
      WorkingDataPointer = (int *) ((char *)WorkingBlockPointer +
            sizeof(struct BlockHeader));
      /* Fill in the data fields with ID numbers and values */
      for (j = 0; j < (i * 10); j++, WorkingDataPointer++) {
         *WorkingDataPointer = j;
         *(WorkingDataPointer + i * 10) = i * 1000 + j;
      }
      /* Remember where to set the link from this block to the next */
      LastBlockPointer = &WorkingBlockPointer->NextBlock;
   }
   /* Set the last block's "next block" pointer to NULL to indicate
      that there are no more blocks */
   WorkingBlockPointer->NextBlock = NULL;

   printf("Average of all elements with ID %d: %u\n",
         IDToFind, FindIDAverage2(IDToFind, BaseArrayBlockPointer));
   exit(0);
}
