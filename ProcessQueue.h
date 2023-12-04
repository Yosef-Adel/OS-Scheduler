#pragma once

#include "headers.h"

struct ProcessQueue
{
    int size;
    int capacity;
    process **processes;
} ProcessQueue;

void initializeProcessQueue(int capacity){
    if(!capacity || capacity<1) capacity = 200;
    if(capacity<200) capacity = 200;
    ProcessQueue.size = 0;
    ProcessQueue.capacity = capacity;
    ProcessQueue.processes = (process **)malloc(sizeof(process *) * capacity);
}

process insertProcess(process *proc){
    if(ProcessQueue.size == ProcessQueue.capacity)
    {
        ProcessQueue.capacity *= 2;
        ProcessQueue.processes = (process **)realloc(ProcessQueue.processes, sizeof(process *) * ProcessQueue.capacity);
    }
    ProcessQueue.processes[ProcessQueue.size] = proc;
    ProcessQueue.size++;
    return *proc;
}

bool removeProcess(process *proc){
    int i;
    process *temp;
    for(i=0; i<ProcessQueue.size; i++)
    {
        if(ProcessQueue.processes[i]->pid == proc->pid)
        {
            temp = ProcessQueue.processes[i];
            ProcessQueue.processes[i] = ProcessQueue.processes[ProcessQueue.size-1];
            ProcessQueue.size--;
            free(temp);
            return 1;
        }
    }
    return 0;
}

void queueRefresh(){
    int i;
    for(i=0; i<ProcessQueue.size; i++)
    {
        if(ProcessQueue.processes[i]->status == 1 || ProcessQueue.processes[i]->status == 2) // started or resumed
            ProcessQueue.processes[i]->remainingtime--;

        else if(ProcessQueue.processes[i]->status == 3) // stopped
            ProcessQueue.processes[i]->waitingtime++;

        else if(ProcessQueue.processes[i]->status == 4) // finished
            removeProcess(ProcessQueue.processes[i]);

    }
}

process *getProcess(int id){
    int i;
    for(i=0; i<ProcessQueue.size; i++)
    {
        if(ProcessQueue.processes[i]->id == id)
            return ProcessQueue.processes[i];
    }
    return NULL;
}

bool freeProcessQueue(){
    int i;
    for(i=0; i<ProcessQueue.size; i++)
        free(ProcessQueue.processes[i]);
    free(ProcessQueue.processes);
    return 1;
}