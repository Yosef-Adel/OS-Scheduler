// TODO implement SRTN scheduling algorithm

#pragma once
#include "priority_queue.h"

bool STRNaddProcess(void *type, process *p);
bool STRNpreempt(void *type);
process *STRNgetNextProcess(void *type);
bool STRNremoveProcess(void *type, process *p);
bool STRNfree(void *type);

bool STRNInitialize(scheduling_algo *current_algorithm)
{
    if (!current_algorithm)
        return false;

    priority_queue *STRNqueue = (priority_queue *)malloc(sizeof(priority_queue));
    createQueue(STRNqueue, 50);

    *current_algorithm = (scheduling_algo){
        STRNqueue,
        STRNaddProcess,
        STRNpreempt,
        STRNgetNextProcess,
        STRNremoveProcess,
        STRNfree,
    };

    return true;
}

bool STRNaddProcess(void *type, process *p)
{
    priority_queue *pq = (priority_queue *)type;
    bool success = pQueueEnqueue(pq, p);
    return success;
}

process *STRNgetNextProcess(void *type)
{
    priority_queue *pq = (priority_queue *)type;
    process *p = getQueuePeek(pq);
    return p;
}

bool STRNpreempt(void *type)
{
    priority_queue *pq = (priority_queue *)type;

    if (getQueuePeek(pq) == NULL)
        return true;

    return curentProcess != getQueuePeek(pq);
}

bool STRNremoveProcess(void *type, process *p)
{
    priority_queue *pq = (priority_queue *)type;
    bool success = pQueueRemove(pq, p);
    return success;
}

bool STRNfree(void *type)
{
    priority_queue *pq = (priority_queue *)type;
    free(pq);
    return true;
}
