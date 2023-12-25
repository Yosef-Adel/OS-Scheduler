//TODO 1. Allocate memory for the memory manager
//TODO 2. deallocate memory for the memory manager
//TODO 3. Create a memory manager initizliaer

///////////////////////////////////////////memoryManager.h///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MemoryList.h"
#include <math.h>

// func def
int alocateMemory(memList *free_list, int memSize);
void deAllocateMemory(memList *free_list, int memStart, int memSize);

void initializeMemoryManager(memList *free_list);

// func imp
// 0 1 2 3  4  5  6  7   8   9   10
// 1 2 4 8 16 32 64 128 256 512 1024

void initializeMemoryManager(memList *free_list) {

  for (int i = 0; i < 11; i++) {

    memList mlist = (memList){.head = NULL, .tail = NULL, .index = i};

    free_list[i] = mlist;
  }

  // initally the mem is a 1024 hole
  addHole(&free_list[10], 0);
}

// Function to find the best memory slot size for a given memory size
int BestSlot(int memSize) 
{
  int list_index = ceil(log2(memSize));
  int memorySlot = pow(2, (int)list_index);
  return memorySlot;
  
}

int alocateMemory(memList *free_list, int memSize) 
{

  int list_index = ceil(log2(memSize));
  int memorySlot = pow(2, (int)list_index);
  int i = list_index;

  while (getminadderess(&free_list[i]) == -1) {
    ++i;
  }
  int slot = pow(2, i); // 64
  int k = i;            // 5

  int addr = -1;

  for (int j = 0; j < i - list_index; j++) { // run 2
    slot /= 2;
    addr = ExtractFirstAddress(&free_list[k]);

    addHole(&free_list[k - 1], addr);
    addHole(&free_list[k - 1], addr + slot);
    k--;
  }

  addr = ExtractFirstAddress(&free_list[k]);
  return addr;
}


