#pragma once
#include "headers.h"

typedef struct priority_queue
{
    int size;
    int capacity;
    process **processes;
} priority_queue;

//// Priority Queue Functions ////
int parent(int i)
{
    return (i - 1) / 2;
}
int left(int i)
{
    return 2 * i + 1;
}
int right(int i)
{
    return 2 * i + 2;
}

void swap(process **x, process **y)
{
    process *temp = *x;
    *x = *y;
    *y = temp;
}

bool HPF_compare(process *p1, process *p2)
{
    if (p1->priority < p2->priority)
    {
        return true;
    }
    else if (p1->priority == p2->priority)
    {
        if (p1->arrival < p2->arrival)
        {
            return true;
        }
        else if (p1->arrival == p2->arrival)
        {
            if (p1->runtime < p2->runtime)
            {
                return true;
            }
            else if (p1->runtime == p2->runtime)
            {
                if (p1->id < p2->id)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool SRTN_compare(process *p1, process *p2)
{
    if (p1->runtime < p2->runtime)
    {
        return false;
    }
    else if (p1->runtime == p2->runtime)
    {
        if (p1->arrival < p2->arrival)
        {
            return false;
        }
        else if (p1->arrival == p2->arrival)
        {
            if (p1->priority < p2->priority)
            {
                return false;
            }
            else if (p1->priority == p2->priority)
            {
                if (p1->id < p2->id)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool compare(process *p1, process *p2)
{

    if (selected_algo == 1)
    {
        return HPF_compare(p1, p2);
    }
    else if (selected_algo == 2)
    {
        return !SRTN_compare(p1, p2);
    }

    return false; // should never reach here
}

void printQueue(priority_queue *q)
{
    for (int i = 0; i < q->size; i++)
    {
        printf("%d ", q->processes[i]->id);
        fflush(stdout);
    }
    printf("\n");
    fflush(stdout);
}

bool createQueue(priority_queue *all_process, int capacity)
{
    if (!all_process)
        return 0;
    if (!capacity)
        capacity = 50;

    all_process->size = 0;
    all_process->capacity = capacity;
    all_process->processes = (process **)calloc(all_process->capacity, sizeof(process *)); // initialize to zero
    return 1;
}

process *getQueuePeek(priority_queue *all_process)
{
    if (all_process->size == 0)
        return NULL;
    return all_process->processes[0];
}

void reheapDown(priority_queue *all_process, int i)
{
    int *size = &all_process->size;
    bool left_ok = left(i) < *size && compare(all_process->processes[left(i)], all_process->processes[i]);
    bool right_ok = right(i) < *size && compare(all_process->processes[right(i)], all_process->processes[i]);

    while (i < *size && (left_ok || right_ok))
    {
        if (left_ok && right_ok)
        {
            if (compare(all_process->processes[left(i)], all_process->processes[right(i)]))
            {
                swap(&all_process->processes[i], &all_process->processes[left(i)]);
                i = left(i);
            }
            else
            {
                swap(&all_process->processes[i], &all_process->processes[right(i)]);
                i = right(i);
            }
        }
        else if (left_ok)
        {
            swap(&all_process->processes[i], &all_process->processes[left(i)]);
            i = left(i);
        }
        else
        {
            swap(&all_process->processes[i], &all_process->processes[right(i)]);
            i = right(i);
        }

        left_ok = left(i) < *size && compare(all_process->processes[left(i)], all_process->processes[i]);
        right_ok = right(i) < *size && compare(all_process->processes[right(i)], all_process->processes[i]);
    }
}

void reheapUp(priority_queue *all_process)
{
    int *size = &all_process->size;

    int i = *size - 1;
    while (i != 0 && compare(all_process->processes[i], all_process->processes[parent(i)]))
    {
        swap(&all_process->processes[i], &all_process->processes[parent(i)]);
        i = parent(i);
    }
}

process *pQueueDequeue(priority_queue *all_process)
{
    if (!all_process || !all_process->processes)
        return NULL;
    int *size = &all_process->size;
    (*size--);
    process *removed = all_process->processes[0];
    all_process->processes[0] = all_process->processes[*size];
    // reheap down
    reheapDown(all_process, 0);
    return removed;
}

bool pQueueEnqueue(priority_queue *all_process, process *p)
{
    if (all_process->capacity == all_process->size)
        return 0;
    int *size = &all_process->size;
    all_process->processes[*size] = p;
    (*size)++;
    // reheap up
    reheapUp(all_process);
    return 1;
}

bool pQueueRemove(priority_queue *all_process, process *p)
{
    int i;
    int *size = &all_process->size;

    for (i = 0; i < *size; i++)
    {
        if (all_process->processes[i] == p)
        {
            swap(&all_process->processes[i], &all_process->processes[*size - 1]);
            (*size)--;
            reheapDown(all_process, i);
            // break;
            return 1;
        }
    }
    return 0;
}

bool freeQueue(priority_queue *all_process)
{
    if (!all_process || !all_process->processes)
        return 0;
    free(all_process->processes);
    return 1;
}