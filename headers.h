#include <stdio.h>      //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef short bool;
#define true 1
#define false 0

#define SHKEY 300


///==============================
//don't mess with this variable//
int * shmaddr;                 //
//===============================



int getClk()
{
    return *shmaddr;
}


/*
 * All process call this function at the beginning to establish communication between them and the clock module.
 * Again, remember that the clock is only emulation!
*/
void initClk()
{
    int shmid = shmget(SHKEY, 4, 0444);
    while ((int)shmid == -1)
    {
        //Make sure that the clock exists
        printf("Wait! The clock not initialized yet!\n");
        sleep(1);
        shmid = shmget(SHKEY, 4, 0444);
    }
    shmaddr = (int *) shmat(shmid, (void *)0, 0);
}


/*
 * All process call this function at the end to release the communication
 * resources between them and the clock module.
 * Again, Remember that the clock is only emulation!
 * Input: terminateAll: a flag to indicate whether that this is the end of simulation.
 *                      It terminates the whole system and releases resources.
*/

void destroyClk(bool terminateAll)
{
    shmdt(shmaddr);
    if (terminateAll)
    {
        killpg(getpgrp(), SIGINT);
    }
}



typedef struct
{
    //these are vars from the process file 
    int id;
    int arrival;
    int runtime;
    int priority;
//======================
    // these vars will help you in further calculations 
    int pid;
    int remainingtime;
    int waitingtime;
    int StartedBefore; // 1 means it was started before, 0 means it has never been started before
    int status;
    /*
    possible status values:
    1 ----> started
    2 ----> resumed
    3 ----> stopped
    4 ----> finished
    */
   
    // here you can add any var you think will be usefull 
} process;

typedef struct
{
    int mtype; // 1 for messages to the scheduler
    process proc; // will use this msgbuff for sending process from the process generator to the scheduler
} msgbuff;

typedef struct scheduling_algo{
    void *type;                  //data structure that implement a certain secheduling algo 
    bool (*addProcess)(void *type, process *proc);  
    bool (*preempt)(void *type); //if need change currant process
    process *(*getNextProcess)(void *type);  // get next process to run
    bool (*removeProcess)(void *type, process *proc);//remove process
    bool (*free)(void *type);//
   

}scheduling_algo; // this work as interface for scheduling, we will implement it's functuions for each algorithm


int selected_algo;
int SJFflag = 0;
int RRquanta = 2;
process *curentProcess=NULL;
scheduling_algo algo;
