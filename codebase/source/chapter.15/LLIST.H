/* Linked list header file. */

#define MAX_TEXT_LENGTH 100   /* longest allowed Text field */
#define SENTINEL  32767       /* largest possible Value field */

struct LinkNode {
   struct LinkNode *NextNode;
   int Value;
   char Text[MAX_TEXT_LENGTH+1];
   /* Any number of additional data fields may by present */
};
	/* The first prototype needs to be changed to support
	   Listing 15-3.c */
struct LinkNode *DeleteNodeAfter(struct LinkNode *);
struct LinkNode *FindNodeBeforeValue(struct LinkNode *, int);
struct LinkNode *InitLinkedList(void);
struct LinkNode *InsertNodeSorted(struct LinkNode *,
   struct LinkNode *);

