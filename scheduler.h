#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "headers.h"
#include "ProcessQueue.h"
#include "RR.h"
// #include "HPF.h"

#include "SRTN.h"
#include "HPF.h"

int previousTimeUsage; // Time when we last utilized the CPU
int idleTime = 0;      // Time CPU was idle in, initially = 0
int finishTime = 0;    // Time when the last process finished, initially = 0

int waitingTime = 0; // waiting time of processes, initially = 0
int numProc = 0;     // number of processes, initially = 0
double avgWTA = 0;   // average weighted turnaround time, initially = 0

scheduling_algo algo;
FILE *outputStats;    // File pointer to the output file, where stats will be written
FILE *memOutputStats; // File pointer to the memory output file, where stats will be written

void schedulerIsForContextSwitch();
void schedulerAreDead(int SIGNUM);
void schedulerPreempt(process *proc);
void schedulerResume(process *proc);
void childAreDead(int SIGNUM);
bool schedulerInitialize(int algo_num, int *msgq_id);
bool getMessage(int msgq_id, msgbuff *message);
void CreateProcess(msgbuff *msg_buffer);
void getQuantumSize();
#endif