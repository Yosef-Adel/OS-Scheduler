#include "headers.h"

#define MAX_PROCESSES 100
int clk_pid;
int scheduler_pid;
int stat_loc;

void clearResources(int);


int main(int argc, char * argv[])
{
    // handle SIGINT signal
    signal(SIGINT, clearResources);
    //-------------------------------------------------------------------------------------
    // Read the input files.
    FILE *file = fopen("processes.txt", "r");
    if (file == NULL){
        printf("Error opening file!\n");
        exit(1);
    }

    // ignore the header line
    char line[30];
    fscanf(file, "%s %s %s %s", line, line, line, line);

    process *processes[MAX_PROCESSES];

    // read the processes from the file and store them in the processes array
    int temp_id, temp_arrival, temp_runtime, temp_priority; 
    int size = 0;

    while ( fscanf(file, "%d %d %d %d", &temp_id, &temp_arrival, &temp_runtime, &temp_priority) != EOF){
        // Allocate memory for the current process
        process *new_process = (process *)malloc(sizeof(process));

        // Check if memory allocation is successful
        if (new_process == NULL)
        {
            printf("Memory allocation failed. Exiting...\n");
            exit(EXIT_FAILURE);
        }
        // Assign the values to the current process
        *new_process = (process){
            .id = temp_id,
            .arrival = temp_arrival,
            .runtime = temp_runtime,
            .priority = temp_priority,
            .remainingtime = temp_runtime,
            .waitingtime = 0,
            .StartedBefore = 0,
            .status = 3,
        };
        // Add the current process to the processes array
        processes[size] = new_process;
        size++;
    }
    fclose(file);
    //-------------------------------------------------------------------------------------
    // Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    printf("Please enter the desired scheduling algorithm:\n");
    printf("1. HPF\n2. SRTN\n3. RR\n");
    fflush(stdout);
    do {
        scanf("%d", &selected_algo);
        if (selected_algo < 1 || selected_algo > 3)
            printf("Invalid input. Please enter a number between 1 and 3.\n");
    } while (selected_algo < 1 || selected_algo > 3);

    if (selected_algo == 3){
        printf("Please enter the desired time quanta:\n");
        fflush(stdout);
        scanf("%d", &RRquanta);
    }
    //-------------------------------------------------------------------------------------
    // create a message queue for sending processes to the scheduler
    key_t key_id;
    int msgq_id, send_val;
    key_id = ftok("keyfile", 65);
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);
    if (msgq_id == -1)
    {
        perror("Error in creating message queue");
        exit(-1);
    }
    printf("Message Queue ID = %d\n", msgq_id);
    fflush(stdout);
    //-------------------------------------------------------------------------------------
    // Initiate and create the scheduler and clock processes.
    clk_pid = fork();
    if(clk_pid < 0){
        printf("Error while forking clk \n");
        fflush(stdout);
        exit(1);
    }

    if(clk_pid == 0){
        execl("clk.out","clk.out",(char *)NULL);
    }else{
        scheduler_pid = fork();
        if(scheduler_pid < 0){
            printf("Error while forking scheduler \n");
            fflush(stdout);
            exit(1);
        }
        if(scheduler_pid == 0){
            /*
            char selectedAlgorithmChar[5];
            sprintf(selectedAlgorithmChar, "%d", selected_algo);
            execl("scheduler.out","scheduler.out",selectedAlgorithmChar,(char *)NULL);
            */
            // TODO: pass the selected algorithm and its parameters to the scheduler
            //execl("scheduler.out","scheduler.out",(char *)NULL);
        }
    }
    //-------------------------------------------------------------------------------------
    // intialize the clock
    initClk();
    //-------------------------------------------------------------------------------------
    // Main Loop
    int next_process_index = 0;

    while(true){
        int current_time = getClk();
        if(next_process_index == size){
            break;
        }
        // this loop to handle the case of multiple processes arriving at the same time
        while (next_process_index < size){
            if(processes[next_process_index]->arrival == current_time){
                msgbuff msg;
                msg.mtype = 1;
                msg.proc = *processes[next_process_index];
                send_val = msgsnd(msgq_id, &msg, sizeof(msg.proc), !IPC_NOWAIT);
                if (send_val == -1)
                {
                    perror("Errror in send");
                }
                next_process_index++;
            }else{
                break;
            }
        }

        printf("current time is %d\n", current_time);
        sleep(1);
        
    }
    //-------------------------------------------------------------------------------------

    printf("Waiting for scheduler to finish\n");
    fflush(stdout);
    int temp_pid = wait(&stat_loc);
    // 5. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    //TODO ask abdelkhalik about if i need to kill the scheduler and clk and free the processes
    destroyClk(true);
    key_t key_id;

    key_id = ftok("keyfile", 65);

    int msgq_id = msgget(key_id, 0666 | IPC_CREAT);
    msgctl(msgq_id, IPC_RMID, (struct msqid_ds *)0);

    exit(0);
}
