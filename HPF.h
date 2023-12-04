// TODO implement HPF

#pragma once
#include "priority_queue.h"

bool HPFaddProcess(void *type, process *p);
bool HPFpreempt(void *type);
process *HPFgetNextProcess(void *type);
bool HPFremoveProcess(void *type, process *p);
bool HPFfree(void *type);

void HPFcast(void *type)
{
    // queue = (priQ *) type;
}

bool HPFInitialize(scheduling_algo *current_algorithm)
{
    if (!current_algorithm)
        return 0;

    priority_queue *HPFqueue = (priority_queue *)malloc(sizeof(priority_queue));
    createQueue(HPFqueue, 50);

    *current_algorithm = (scheduling_algo){
        HPFqueue,
        HPFaddProcess,
        HPFpreempt,
        HPFgetNextProcess,
        HPFremoveProcess,
        HPFfree,

    };
    return 1;
}

bool HPFaddProcess(void *type, process *p)
{
    priority_queue *pq = (priority_queue *)type;
    bool success = pQueueEnqueue(pq, p);
    return success;
}

process *HPFgetNextProcess(void *type)
{
    priority_queue *pq = (priority_queue *)type;
    process *p = getQueuePeek(pq);
    return p;
}

bool HPFpreempt(void *type)
{ // take care of the process that is currently running
    if (curentProcess == NULL)
        return true;
    return false;
}

bool HPFremoveProcess(void *type, process *p)
{
    priority_queue *pq = (priority_queue *)type;
    bool success = pQueueRemove(pq, p);
    return success;
}

bool HPFfree(void *type)
{
    priority_queue *pq = (priority_queue *)type;
    free(pq);
    return true;
}
