#include <limits.h>
#include <math.h>
#include <stdio.h>
struct node {
  int addresss;
  struct node *next;
};

typedef struct {
  int index;
  struct node *head;
  struct node *tail;
} memList;

// function def
void addHole(memList *mlist, int addresss);
int ExtractFirstAddress(memList *memList);
void sortHoles(memList *mlist);
int getminadderess(memList *memlist);

// function imp

void addHole(memList *mlist, int addresss) {
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

void deleteHole(memList *mlist, int addresss) {

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

int getminadderess(memList *mlist) {

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

int ExtractFirstAddress(memList *mList) {
  int min = getminadderess(mList);
  deleteHole(mList, min);
  return min;
}

void display(memList *mlist) {

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

void sortHoles(memList *mlist) {
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

int compactList(memList *mlist) {
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
