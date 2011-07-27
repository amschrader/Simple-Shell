/**
 * Allyson Schrader amschrad@ucsc.edu
 * List.h
 * CMPS101 PA5
 * 5.29.08
 */
#if !defined(_LIST_H_INCLUDE_)
#define _LIST_H_INCLUDE_

typedef enum {FALSE = (0!=0), TRUE = (0==0)} bool;

typedef struct Node{
   int data;
   struct Node* next;
   struct Node* prev;
} Node, * NodeRef;

typedef struct List{
   NodeRef front;
   NodeRef rear;
   NodeRef curr;
   int length;
} List, *ListRef;


/*** Constructors-Destructors ***/
ListRef newList(void);
void freeList(ListRef* pL);

/*** Access functions ***/
int isEmpty(ListRef L);
int offEnd(ListRef L);
int atFirst(ListRef L);
int atLast(ListRef L);
int getFirst(ListRef L);
int getLast(ListRef L);
int getCurrent(ListRef L);
int getLength(ListRef L);
int equals(ListRef A, ListRef B);

/*** Manipulation procedures ***/
void makeEmpty(ListRef L);
void moveFirst(ListRef L);
void moveLast(ListRef L);
void movePrev(ListRef L);
void moveNext(ListRef L);
void insertBeforeFirst(ListRef L, int data);
void insertAfterLast(ListRef L, int data);
void insertBeforeCurrent(ListRef L, int data);
void insertAfterCurrent(ListRef L, int data);
void insertInOrder(ListRef L, int data);
void deleteFirst(ListRef L);
void deleteLast(ListRef L);
void deleteCurrent(ListRef L);
void makeListNull(ListRef L);

/*** Other operations ***/
void printList(FILE* out, ListRef L);
ListRef copyList(ListRef L);

#endif
