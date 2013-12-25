/* Program to search through an array spanning a linked list of
   variable-sized blocks for all entries with a specified ID number,
   and return the average of the values of all such entries. Each of
   the variable-sized blocks may contain any number of data entries,
   stored as an array of structures within the block.
*/
         
#include <stdio.h>
#ifdef __TURBOC__
#include <alloc.h>
#else
#include <malloc.h>
#endif

void main(void);
void exit(int);
unsigned int FindIDAverage(unsigned int, struct BlockHeader *);

/* Structure that starts each variable-sized block */
struct BlockHeader {
   struct BlockHeader *NextBlock; /* pointer to next block, or NULL
                                    if this is the last block in the
                                    linked list */
   unsigned int BlockCount;      /* the number of DataElement entries
                                    in this variable-sized block */
};

/* Structure that contains one element of the array we'll search */
struct DataElement {
   unsigned int ID;     /* ID # for array entry */
   unsigned int Value;  /* value of array entry */
};

void main(void) {
   int i,j;
   unsigned int IDToFind;
   struct BlockHeader *BaseArrayBlockPointer,*WorkingBlockPointer;
   struct DataElement *WorkingDataPointer;
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
            sizeof(struct DataElement) * i * 10)) == NULL) {
         exit(1);
      }
      /* Set the # of data elements in this block */
      WorkingBlockPointer->BlockCount = i * 10;
      /* Link the new block into the chain */
      *LastBlockPointer = WorkingBlockPointer;
      /* Point to the first data field */
      WorkingDataPointer =
            (struct DataElement *) ((char *)WorkingBlockPointer +
            sizeof(struct BlockHeader));
      /* Fill in the data fields with ID numbers and values */
      for (j = 0; j < (i * 10); j++, WorkingDataPointer++) {
         WorkingDataPointer->ID = j;
         WorkingDataPointer->Value = i * 1000 + j;
      }
      /* Remember where to set the link from this block to the next */
      LastBlockPointer = &WorkingBlockPointer->NextBlock;
   }
   /* Set the last block's "next block" pointer to NULL to indicate
      that there are no more blocks */
   WorkingBlockPointer->NextBlock = NULL;

   printf("Average of all elements with ID %d: %u\n",
         IDToFind, FindIDAverage(IDToFind, BaseArrayBlockPointer));
   exit(0);
}

/* Searches through the array of DataElement entries spanning the
   linked list of variable-sized blocks starting with the block
   pointed to by BlockPointer for all entries with IDs matching
   SearchedForID, and returns the average value of those entries. If
   no matches are found, zero is returned */
unsigned int FindIDAverage(unsigned int SearchedForID,
      struct BlockHeader *BlockPointer)
{
   struct DataElement *DataPointer;
   unsigned int IDMatchSum;
   unsigned int IDMatchCount;
   unsigned int WorkingBlockCount;

   IDMatchCount = IDMatchSum = 0;
   /* Search through all of the linked blocks until the last block
      (marked with a NULL pointer to the next block) has been
      searched */
   do {
      /* Point to the first DataElement entry within this block */
      DataPointer =
            (struct DataElement *) ((char *)BlockPointer +
            sizeof(struct BlockHeader));
      /* Search through all the DataElement entries within this block
         and accumulate data from all that match the desired ID */
      for (WorkingBlockCount=0;
            WorkingBlockCount<BlockPointer->BlockCount;
            WorkingBlockCount++, DataPointer++) {
         /* If the ID matches, add in the value and increment the
            match counter */
         if (DataPointer->ID == SearchedForID) {
            IDMatchCount++;
            IDMatchSum += DataPointer->Value;
         }
      }
   /* Point to the next block, and continue so long as that pointer
      isn't NULL */
   } while ((BlockPointer = BlockPointer->NextBlock) != NULL);

   /* Calculate the average of all matches */
   if (IDMatchCount == 0)
      return(0);            /* avoid division by 0 */
   else
      return(IDMatchSum / IDMatchCount);
}

