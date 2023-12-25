#include <limits.h>
#include <math.h>
#include <stdio.h>
struct node 
{
  int addresss; // address of the node
  struct node *next; // pointer to the next node
};

// Definition of a memory list structure
typedef struct 
{
  int index;          // Index of the memory list
  struct node *head;  // Pointer to the head of the list
  struct node *tail;  // Pointer to the tail of the list
} memList;

// function def
void addHole(memList *mlist, int addresss);
int ExtractFirstAddress(memList *memList);
void sortHoles(memList *mlist);
int getminadderess(memList *memlist);

// function imp

/**

 * This function creates a new node with the given address and adds it to the memory list.
 * If the memory list is empty, the new node becomes the head and tail of the list.
 * Otherwise, the new node is appended to the tail of the list.
 * After adding the hole, the memory list is sorted.
 * 
 * @param mlist Pointer to the memory list.
 * @param addresss The address of the hole to be added.
 */
void addHole(memList *mlist, int addresss) 
{
  struct node *newNode = (struct node *)malloc(sizeof(struct node));

  newNode->addresss = addresss;
  newNode->next = NULL;

  if (mlist->head == NULL) {
    mlist->head = newNode;
    mlist->tail = newNode;
  } else {
    mlist->tail->next = newNode;
    mlist->tail = newNode;
  }

  sortHoles(mlist);
}

/**
 * Deletes a hole from the memory list based on the given address.
 * If the hole with the specified address is found, it is removed from the list.
 * The memory list is then sorted after the deletion.
 *
 * @param mlist The memory list.
 * @param addresss The address of the hole to be deleted.
 */
void deleteHole(memList *mlist, int addresss) 
{

  struct node *temp = mlist->head;
  struct node *prev;

  if (temp != NULL && temp->addresss == addresss) {
    mlist->head = temp->next;
    free(temp);
    return;
  }

  while (temp != NULL && temp->addresss != addresss) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL)
    return;

  prev->next = temp->next;
  free(temp);
  sortHoles(mlist);
}

