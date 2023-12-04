#pragma once
#include "headers.h"

typedef struct priority_queue
{
    // TODO: add see if you can intialize the schedulingAlgorithm here
    int size;
    int capacity;
    process **processes;
} priority_queue;

//// Priority Queue Functions ////
int parent(int i){
    return (i-1)/2;
}
int left(int i){
    return 2*i+1;
}
int right(int i){
    return 2*i+2;
}

void swap(process **p1, process **p2){
    process *temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

bool HPF_compare(process *p1, process *p2){
    if(p1->priority < p2->priority){
        return true;
    }
    else if(p1->priority == p2->priority){
        if(p1->arrival < p2->arrival){
            return true;
        }
        else if(p1->arrival == p2->arrival){
            if(p1->runtime < p2->runtime){
                return true;
            }
            else if(p1->runtime == p2->runtime){
                if(p1->id < p2->id){
                    return true;
                }
            }
        }
    }
    return false;
}


bool SRTN_compare(process *p1, process *p2){
    if(p1->runtime < p2->runtime){
        return true;
    }
    else if(p1->runtime == p2->runtime){
        if(p1->arrival < p2->arrival){
            return true;
        }
        else if(p1->arrival == p2->arrival){
            if(p1->priority < p2->priority){
                return true;
            }
            else if(p1->priority == p2->priority){
                if(p1->id < p2->id){
                    return true;
                }
            }
        }
    }
    return false;
}

bool compare(process *p1, process *p2 , int schedulingAlgorithm){

    if( schedulingAlgorithm ==1){
        return HPF_compare(p1,p2);
    }else if(schedulingAlgorithm ==2){
        return SRTN_compare(p1,p2);
    }

    return false; // should never reach here
}

void printQueue(priority_queue *q){
    for(int i=0; i<q->size; i++){
        printf("%d ", q->processes[i]->id);
        fflush(stdout);
    }
    printf("\n");
    fflush(stdout);
}

bool createQueue(priority_queue *q, int capacity){
    if(!q) return 0;
    if(!capacity) capacity=50;
    q = (priority_queue *)malloc(sizeof(priority_queue));
    q->size = 0;
    q->capacity = capacity;

    q->processes = (process **)malloc(capacity * sizeof(process *)); // array of pointers to processes may produce errors
    if(q->processes == NULL){
        return 0;
    }
    return 1;
}

process *getpeek(priority_queue *q){
    if(q->size == 0){
        return NULL;
    }
    return q->processes[0];
}

void reheapDown(priority_queue *all_process, int i, int schedulingAlgorithm){
    int *size = &all_process->size;
    bool left_ok = left(i) < *size && compare(all_process->processes[left(i)], all_process->processes[i], schedulingAlgorithm);
    bool right_ok = right(i) < *size && compare(all_process->processes[right(i)], all_process->processes[i], schedulingAlgorithm);

    while (i < *size && (left_ok || right_ok))
    {
        if (left_ok && right_ok)
        {
            if (compare(all_process->processes[left(i)], all_process->processes[right(i)], schedulingAlgorithm))
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
        left_ok = left(i) < *size && compare(all_process->processes[left(i)], all_process->processes[i], schedulingAlgorithm);
        right_ok = right(i) < *size && compare(all_process->processes[right(i)], all_process->processes[i], schedulingAlgorithm);
    }
   
    
}

void reheapUp(priority_queue *all_process, int i, int schedulingAlgorithm){
    int *size = &all_process->size;
    while (i > 0 && compare(all_process->processes[i], all_process->processes[parent(i)], schedulingAlgorithm))
    {
        swap(&all_process->processes[i], &all_process->processes[parent(i)]);
        i = parent(i);
    }
}

process *dequeue(priority_queue *all_process, int schedulingAlgorithm){
    if(!all_process || all_process->size == 0)
        return NULL;
    
    // potiential error
    process *p = all_process->processes[0];
    all_process->processes[0] = all_process->processes[all_process->size - 1];
    all_process->size--;
    reheapDown(all_process, 0, schedulingAlgorithm);
    return p;
}

bool enqueue(priority_queue *all_process, process *p, int schedulingAlgorithm){
    if(!all_process || !p)
        return 0;
    if(all_process->size == all_process->capacity){
        return 0;
    }
    all_process->processes[all_process->size] = p;
    all_process->size++;
    reheapUp(all_process, all_process->size - 1, schedulingAlgorithm);
    return 1;
}

bool removeProcess(priority_queue *all_process, process *p, int schedulingAlgorithm){
    if(!all_process || !p)
        return 0;

    int i;

    for(i=0; i<all_process->size; i++){
        if(all_process->processes[i] == p){
            swap(&all_process->processes[i], &all_process->processes[all_process->size - 1]);
            all_process->size--;
            reheapDown(all_process, i, schedulingAlgorithm);
            
            return 1;
        }
    }

    return 0;

}

bool freeQueue(priority_queue *q){
    if(!q) return 0;
    free(q->processes);
    free(q);
    return 1;
}