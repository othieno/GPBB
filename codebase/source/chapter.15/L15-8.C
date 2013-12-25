/* Sample linked list program. Tested with Borland C++. */

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include "llist.h"

void main()
{
   int Done = 0, Char, TempValue;
   struct LinkNode *TempPtr, *ListPtr, *TempPtr2;
   char TempBuffer[MAX_TEXT_LENGTH+3];

   if ((ListPtr = InitLinkedList()) == NULL) {
       printf("Out of memory\n");
       exit(1);
    }

   while (!Done) {
      printf("\nA=add; D=delete; F=find; L=list all; Q=quit\n>");
      Char = toupper(getche());
      printf("\n");
      switch (Char) {
         case 'A':               /* add a node */
            if ((TempPtr = malloc(sizeof(struct LinkNode))) == NULL)
            {  printf("Out of memory\n");
               exit(1);
            }
            printf("Node value: ");
            scanf("%d", &TempPtr->Value);
            if ((FindNodeBeforeValue(ListPtr,TempPtr->Value))!=NULL)
            {  printf("*** value already in list; try again ***\n");
               free(TempPtr);
            } else {
               printf("Node text: ");
               TempBuffer[0] = MAX_TEXT_LENGTH;
					cgets(TempBuffer);
               strcpy(TempPtr->Text, &TempBuffer[2]);
					InsertNodeSorted(ListPtr, TempPtr);
               printf("\n");
            }
            break;
         case 'D':               /* delete a node */
            printf("Value field of node to delete: ");
            scanf("%d", &TempValue);
            if ((TempPtr = FindNodeBeforeValue(ListPtr, TempValue))
                 != NULL) {
               TempPtr2 = TempPtr->NextNode; /* -> node to delete */
               DeleteNodeAfter(TempPtr);     /* delete it */
               free(TempPtr2);               /* free its memory */
            } else {
               printf("*** no such value field in list ***\n");
            }
            break;
         case 'F':               /* find a node */
            printf("Value field of node to find: ");
            scanf("%d", &TempValue);
            if ((TempPtr = FindNodeBeforeValue(ListPtr, TempValue))
                  != NULL)
               printf("Value: %d\nText: %s\n",
                 TempPtr->NextNode->Value, TempPtr->NextNode->Text);
            else
               printf("*** no such value field in list ***\n");
            break;
         case 'L':               /* list all nodes */
            TempPtr = ListPtr->NextNode;  /* point to first node */
            if (TempPtr == ListPtr) {     /* empty if at sentinel */
               printf("*** List is empty ***\n");
            } else {
               do {
                  printf("Value: %d\n  Text: %s\n", TempPtr->Value,
                        TempPtr->Text);
                  TempPtr = TempPtr->NextNode;
               } while (TempPtr != ListPtr);
            }
            break;
         case 'Q':
            Done = 1;
            break;
         default:
            break;
      }
   }
}

