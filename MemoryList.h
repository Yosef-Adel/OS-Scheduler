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


/**
 * This function returns the minimum address in the memory list.
 * It iterates through the list and compares each address with the current minimum.
 * If a smaller address is found, it updates the minimum.
 * If the list is empty, it returns -1.
 * 
 * @param mlist Pointer to the memory list.
 * @return The minimum address in the memory list, or -1 if the list is empty.
 */
int getminadderess(memList *mlist) 
{

  int min = INT_MAX;

  struct node *ptr = mlist->head;

  while (ptr != NULL) {

    if (ptr->addresss < min)
      min = ptr->addresss;
    ptr = ptr->next;
  }

  if (min != INT_MAX)
    return min;
  else
    return -1;
}

int ExtractFirstAddress(memList *mList) 
{
  int min = getminadderess(mList);
  deleteHole(mList, min);
  return min;
}
///////////////////////////////////check output ////////////////////////////////
void display(memList *mlist) 
{

  struct node *current = mlist->head;
  if (current == NULL) {
    printf("list is empty");
    return;
  }
  while (current != NULL) {
    printf("%d", current->addresss);
    current = current->next;
  }
  printf("\n");
}
/////////////////////////////////////////////////////////////////////////////////

void sortHoles(memList *mlist) 
{
  int a;

  struct node *h = mlist->head;

  struct node *temp1;
  struct node *temp2;

  for (temp1 = h; temp2 != NULL; temp1 = temp1->next) {
    for (temp2 = temp1->next; temp2 != NULL; temp2 = temp2->next) {
      if (temp2->addresss < temp1->addresss) {
        a = temp1->addresss;
        temp1->addresss = temp2->addresss;
        temp2->addresss = a;

      }
    }
  }
}

int compactList(memList *mlist) 
{
  struct node *temp1;
  struct node *temp2;

  for (temp1 = mlist->head; temp1 != NULL; temp1 = temp1->next) {
    for (temp2 = temp1->next; temp2 != NULL; temp2 = temp2->next) {
      if (temp1->addresss % (int)(2 * pow(2, mlist->index)) == 0 &&
          temp1->addresss + pow(2, mlist->index) == temp2->addresss) {

        // compact
        int addr = temp1->addresss;
        deleteHole(mlist, temp1->addresss);
        deleteHole(mlist, temp2->addresss);
        return addr;
      }
    }
  }
  return -1;
}
