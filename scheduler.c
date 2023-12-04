#include "scheduler.h"

void getQuantumSize()
{
    printf("Please enter quantum size: \n");
    fflush(stdout);
    scanf("%d", &RRquanta);
    if (RRquanta < 1)
    {
        printf("Invalid input entered. Taking default value of 2 instead.\n");
        fflush(stdout);
        RRquanta = 2;
    }
}

void schedulerIsForContextSwitch()
{
    // Test implementation for schedulerIsForContextSwitch
    // TODO: Add test code here
    printf("context\n");
    fflush(stdout);

    bool check = algo.preempt(algo.type);
    if (check)
    {
        schedulerPreempt(curentProcess);
        process *next = algo.getNextProcess(algo.type);
        schedulerResume(next);
    }
}

void CreateProcess(msgbuff *msg_buffer)
{
    int pid = fork();
    // Child (process)
    if (pid == 0)
    {
        char pRemainingTime[10];
        sprintf(pRemainingTime, "%d", msg_buffer->proc.remainingtime);
        execl("process.out", "process.out", pRemainingTime, (char *)NULL);
    }
    else
    {
        msg_buffer->proc.pid = pid;
        // Making the process stopped (once it enters) and check after if ready
        kill(pid, SIGTSTP);
        // insert in the ProcessQueue
        process *ProcessQueueProcessEntry = ProcessQueueInsert(&msg_buffer->proc);
        // Using the chosen algorithm on the created process
        algo.addProcess(algo.type, ProcessQueueProcessEntry);
        printf("Created process %d\n", msg_buffer->proc.id);
        fflush(stdout);
        schedulerIsForContextSwitch();
    }
}

void schedulerAreDead(int SIGNUM)
{
    printf("schedulerAreDead \n");
    // closing the opened file
    fclose(outputStats);
    printf("schedulerAreDead \n");
    fflush(stdout);

    // Calculating the utilization stats
    outputStats = fopen("schedulerPerf.perf", "w");
    printf("schedulerAreDead%d \n", idleTime);
    fflush(stdout);
    double utilization = 1 - ((double)idleTime / finishTime); // calculates the utilization percentage
    double avgWaitingTime = (double)waitingTime / numProc;
    // Printing the utilization stats in another file
    fprintf(outputStats, "CPU utilization = %.2f%%\n", utilization * 100);
    fprintf(outputStats, "Avg WTA = %.2f\n", avgWTA);
    fprintf(outputStats, "Avg Waiting = %.2f\n", avgWaitingTime);
    fflush(outputStats);
    fclose(outputStats);

    // TODO: free the rest of the stuff here
    ProcessQueueFree();
    algo.free(algo.type); // free current algorithm
    // Removing message queue
    key_t key_id = ftok("keyfile", 65);
    int msgq_id = msgget(key_id, 0666 | IPC_CREAT);
    msgctl(msgq_id, IPC_RMID, (struct msqid_ds *)0);
    // Installing the signal handler again to trap SIGINT again
    signal(SIGINT, schedulerAreDead);
}

void childAreDead(int SIGNUM)
{
    // Test implementation for childAreDead
    // TODOt code here

    int procStatus;
    int procID = wait(&procStatus);
    printf("delete process %d\n", procID);
    fflush(stdout);

    process *proc = ProcessQueueGetProcess(procID);
    algo.removeProcess(algo.type, proc); // removing process from algorithm queue
    numProc++;                           // increasing number of processes
    int turnaroundTime = getClk() - proc->arrival;
    double weightedTurnaroundTime = turnaroundTime / (double)proc->runtime;
    // double avgWeightedTurnaroundTime = avgWTA; // temporary holder
    avgWTA = (weightedTurnaroundTime + (numProc - 1) * avgWTA) / numProc;
    // Method of calucation: similar to that of calculating the GPA
    // getting the total WTA by multiplying the number of processes (excluding the terminating one)
    // by the avgWTA. Then we add the new WTA, and divide by the total number of processes.
    waitingTime += proc->waitingtime;
    previousTimeUsage = getClk(); // updating the utilization time
    if (proc->remainingtime < 0)
        proc->remainingtime = 0;
    // Printing process info

    fprintf(outputStats, "At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",
            getClk(), proc->id, proc->arrival, proc->runtime, proc->remainingtime, proc->waitingtime, turnaroundTime, weightedTurnaroundTime);
    fflush(outputStats);
    ProcessQueueRemove(proc);
    curentProcess = NULL;
    printf("delete process %d\n", procID);
    fflush(stdout);
    schedulerIsForContextSwitch();
    finishTime = getClk();
}

void schedulerPreempt(process *proc)
{
    // Test implementation for schedulerPreempt
    // TODO: Add test code here
    printf("Preempt \n");
    fflush(stdout);
    if (!proc)

        return;
    printf("Preempt 2 %d\n", proc->pid);
    fflush(stdout);
    // printf("Prempted process %d\n",curentProcess->id);
    proc->status = 3;         // Stopped process
    proc->StartedBefore = 1;  // Marking the process as ran before
    kill(proc->pid, SIGTSTP); // Stopping the process
    // Printing process info
    fprintf(outputStats, "At time %d process %d stopped arr %d total %d remain %d wait %d\n",
            getClk(), proc->id, proc->arrival, proc->runtime, proc->remainingtime, proc->waitingtime);
    fflush(outputStats);
    previousTimeUsage = getClk(); // starting from now, till another process utilizes the CPU
    // that will be the idle time.
}

void schedulerResume(process *proc)
{
    // Test implementation for schedulerResume
    // TODO: Add test code here
    printf("resume\n");
    fflush(stdout);
    // Setting the current running process
    curentProcess = proc;
    if (!proc)
        return;
    printf("resume %d\n", proc->pid);
    fflush(stdout);
    proc->status = 2; // Marking the status as running
    // Continue the process, SIGCONT to update waiting times once the process is resumed
    kill(proc->pid, SIGCONT);
    // Printing process info
    // Remember:
    // int StartedBefore; // 1 means it was started before, 0 means it has never been started before
    bool FirstTime = false; // a flag that shows whether process is resuming or starting
    // false means resuming
    if (proc->StartedBefore == 0)
    { // then it is now starting

        FirstTime = true;
    }
    fprintf(outputStats, "At time %d process %d %s arr %d total %d remain %d wait %d\n",
            getClk(), proc->id, (FirstTime) ? "started" : "resumed", proc->arrival, proc->runtime, proc->remainingtime, proc->waitingtime);
    fflush(outputStats);
    // Updating the idleTime for the scheduler
    idleTime += getClk() - previousTimeUsage;
    previousTimeUsage = getClk();
}

bool schedulerInitialize(int algo_num, int *msgq_id)
{
    // Test implementation for schedulerInitialize
    // TODO: Add test code here
    // Opening the file where we will output the process updates
    outputStats = fopen("scheduler.log", "w");
    // Printing the opening statement
    fprintf(outputStats, "#At time x process y state arr w total z remain y wait k\n");
    fflush(outputStats);
    /* sigaction to handle SIGCHLD signal
    whenever a process dies, it calls childAreDead
    (sigaction since SIGCHLD cannot be handled directly)
    */
    struct sigaction chld_handle;
    chld_handle.sa_handler = childAreDead;
    sigemptyset(&chld_handle.sa_mask);
    chld_handle.sa_flags = SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &chld_handle, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
    signal(SIGINT, schedulerAreDead);
    initClk();
    previousTimeUsage = 0;
    // Calling the initializer of each algorithm
    // HPF
    if (algo_num == 1)
    {
        HPFInitialize(&algo);
    }
    // STRN
    else if (algo_num == 2)
    {
        //
    }
    // RR
    else if (algo_num == 3)
    {
        ///
    }
    ProcessQueueInitialize(200);
    // Creating the message queue
    key_t key_id = ftok("keyfile", 65);
    int msgq_idTemp = msgget(key_id, 0666 | IPC_CREAT);
    if (msgq_idTemp == -1)
    {
        printf("Error in create msgq_idTemp");
        fflush(stdout);
        return false;
    }
    *msgq_id = msgq_idTemp;
    return true;
}

bool getMessage(int msgq_id, msgbuff *message)
{
    // Test implementation for getMessage
    // TODO: Add test code here
    // Doesn't wait for the process generator to send message
    // Doesn't wait for the process generator to send message
    int rec_val = msgrcv(msgq_id, message, sizeof(process), 0, IPC_NOWAIT);
    if (rec_val == -1)
        return false;

    return true;
}

int main(int argc, char *argv[])
{
    int algo = atoi(argv[1]);
    int msgq_id;
    schedulerInitialize(algo, &msgq_id);
    msgbuff msgq_buffer;
    int now, previous_time = -1;
    printf("msgq_id= %d\n", msgq_id);
    fflush(stdout);
    while (1)
    {
        now = getClk();
        if (getMessage(msgq_id, &msgq_buffer))
        {
            CreateProcess(&msgq_buffer);
            while (getMessage(msgq_id, &msgq_buffer))
            {
                CreateProcess(&msgq_buffer);
            }
            if (now > previous_time)
            {
                ProcessQueueRefresh();
            }
            schedulerIsForContextSwitch();
            if (now > previous_time)
            {
                previous_time = now;
            }
        }
        if (now > previous_time)
        {
            ProcessQueueRefresh();
            schedulerIsForContextSwitch();
            previous_time = now;
        }
    }

    // TODO implement the scheduler :)

    // upon termination release the clock resources.

    destroyClk(true);

    return 0;
}
