#pragma once

#include "headers.h"

struct ProcessQueue{
    int size;       // actual used sized
    int capacity;   // maximum size available for usage
    process **array;
} ProcessQueue;

void ProcessQueueInitialize(int inputSize) {

    if(!inputSize || inputSize<1) inputSize = 200;
    if(inputSize<200) inputSize = 200;

    ProcessQueue.array = malloc(inputSize*sizeof(process));
    ProcessQueue.size = 0;
    ProcessQueue.capacity = inputSize;

}

process* ProcessQueueInsert(process* p) {

    if(ProcessQueue.size == ProcessQueue.capacity) {
        ProcessQueue.capacity = ProcessQueue.capacity * 2;
        ProcessQueue.array = realloc(ProcessQueue.array,ProcessQueue.capacity*sizeof(process));
    }
    process* to_insert = (process*) malloc(sizeof(process));
    *to_insert = *p;

    ProcessQueue.array[ProcessQueue.size++] = to_insert;
    return to_insert;

}

bool ProcessQueueRemove(process* p) {
    int id = p->id;
    process* remove = NULL;

    for(int i=0;i<ProcessQueue.size;i++) {
        if(ProcessQueue.array[i]->id == id) {
            remove = ProcessQueue.array[i];
            ProcessQueue.array[i] = ProcessQueue.array[ProcessQueue.size-1];
            ProcessQueue.size--;
            break;
        }
    }
    if(remove) {
        free(remove);
        return 1;
    }
    return 0;
}

void ProcessQueueRefresh() {

    for(int i=0;i<ProcessQueue.size;i++) {
        if(ProcessQueue.array[i]->status == 1 || ProcessQueue.array[i]->status==2) { //running process
            ProcessQueue.array[i]->remainingtime--;
        }
        else if(ProcessQueue.array[i]->status == 3) { //waiting process
            ProcessQueue.array[i]->waitingtime++;
        }
        else if(ProcessQueue.array[i]->status == 4) {
            ProcessQueueRemove(ProcessQueue.array[i]);
        }
    }

}

process* ProcessQueueGetProcess(int id) {
    process *p = NULL;
    
    for(int i=0;i<ProcessQueue.size;i++) {
        if(ProcessQueue.array[i]->pid == id) {
            p = ProcessQueue.array[i];
            break;
        }
    }

    // if no process with such ID exists, NULL is returned (must check on NULL)
    return p;
}

bool ProcessQueueFree() {

    for(int i=0;i<ProcessQueue.size;i++) free(ProcessQueue.array[i]);

    free(ProcessQueue.array);

    return 1;
}