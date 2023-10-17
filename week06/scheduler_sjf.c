#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#define PS_MAX 10


typedef struct
{
    int idx;                     
    int at, bt, rt, wt, ct, tat; 
    int burst, arrival_time;     
} ProcessData;

int running_process = -1; 

unsigned total_time; 
ProcessData data[PS_MAX]; 
pid_t ps[PS_MAX]; 
unsigned data_size;

void read_file(FILE *file)
{
    char line[1000];
    data_size = 0;

    while (data_size < PS_MAX && fgets(line, sizeof(line), file) != NULL)
    {
        int at, bt;
        if (sscanf(line, "%d %d %d", &data_size, &at, &bt) == 3)
        {
            data[data_size].idx = data_size;
            data[data_size].rt = data[data_size].bt;
            data[data_size].at = at;
            data[data_size].bt = bt;
            data[data_size].wt = 0;
            data[data_size].ct = 0;
            data[data_size].tat = 0;
            data[data_size].arrival_time = at;
            data[data_size].burst = bt; 
            data_size++;
        }
        }

    for (int i = 0; i < PS_MAX; i++)
    {
        ps[i] = 0;
    }
}

void resume(pid_t process)
{
    if (process > 0)
    {
        if (kill(process, SIGCONT) == -1)
        {
            perror("Error sending SIGCONT to the process");
        }
    }
    else
    {
    
        fprintf(stderr, "Process is not created yet or has terminated.\n");
    }
}
void suspend(pid_t process)
{
    if (process > 0)
    {
        if (kill(process, SIGTSTP) == -1)
        {
            perror("Error sending SIGTSTP to the process");
        }
    }
    else
    {
      
        fprintf(stderr, "Process is not created yet or has terminated.\n");
    }
}
void terminate(pid_t process)
{
    
    if (process > 0)
    {
        if (kill(process, SIGTERM) == -1)
        {
            perror("Error sending SIGTERM to the process");
        }
    }
    else
    {
     
        fprintf(stderr, "Process is not created yet or has terminated.\n");
    }
}


void create_process(int new_process) {
    if (running_process != -1) {
        suspend(running_process);
    }

    pid_t child_pid = fork();
    if (child_pid == 0) {
        char process_idx_str[10];
        sprintf(process_idx_str, "%d", new_process);
        char* args[] = {"./worker", process_idx_str, NULL};
        execvp(args[0], args);
    } else if (child_pid > 0) {
        ps[new_process] = child_pid;
        running_process = new_process;
        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", new_process, data[new_process].burst);
    }
}

ProcessData find_next_process() {
        int shortest_burst = -1;
    int shortest_idx = -1;

    for (int i = 0; i < data_size; i++) {
        if (data[i].burst > 0) {
            if (shortest_burst == -1 || data[i].burst < shortest_burst) {
                shortest_burst = data[i].burst;
                shortest_idx = i;
            }
        }
    }

    if (shortest_idx == -1) {
        printf("No process found for execution.\n");
        exit(EXIT_FAILURE);
    }

    return data[shortest_idx];
}

void report() {
    printf("Simulation results.....\n");
	int sum_wt = 0;
	int sum_tat = 0;
	for (int i=0; i< data_size; i++){
		printf("process %d: \n", i);
		printf("	at=%d\n", data[i].arrival_time);
		printf("	bt=%d\n", data[i].bt);
		printf("	ct=%d\n", data[i].ct);
		printf("	wt=%d\n", data[i].wt);
		printf("	tat=%d\n", data[i].tat);
		printf("	rt=%d\n", data[i].rt);
		sum_wt += data[i].wt;
		sum_tat += data[i].tat;
	}

	printf("data size = %d\n", data_size);
	float avg_wt = (float)sum_wt/data_size;
	float avg_tat = (float)sum_tat/data_size;
	printf("Average results for this run:\n");
	printf("	avg_wt=%f\n", avg_wt);
	printf("	avg_tat=%f\n", avg_tat);
}

void check_burst() {
    for (int i = 0; i < data_size; i++) {
        if (data[i].burst > 0) {
            return;
        }
    }
    report();
    exit(EXIT_SUCCESS);
}


// void schedule_handler(int signum)
// {
//     total_time++;

//     if (running_process >= 0)
//     {
//         data[running_process].burst--;

//         printf("Scheduler: Runtime: %u seconds\n", total_time);
//         printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);

//         if (data[running_process].burst == 0)
//         {
//             printf("Scheduler: Terminating Process %d (Remaining Time: 0)\n", running_process);
//             terminate(ps[running_process]);

//             waitpid(ps[running_process], NULL, 0);

//             data[running_process].ct = total_time;
//             data[running_process].tat = data[running_process].ct - data[running_process].at;
//             data[running_process].wt = data[running_process].tat - data[running_process].bt;

//             data[running_process].at = 10000000;

//             running_process = -1;
//         }
//     }
//     check_burst();
//     ProcessData next_process = find_next_process();

//     if (!(next_process.idx == running_process))
//     {
//         running_process = next_process.idx;

//         printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);

//         create_process(running_process);

//         data[running_process].rt = total_time - data[running_process].at;

//     }

// }

void schedule_handler(int signum) {
    total_time++;

    if (running_process >= 0) {
        data[running_process].burst--;

        printf("Scheduler: Runtime: %u seconds\n", total_time);
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);

        if (data[running_process].burst == 0) {
            printf("Scheduler: Terminating Process %d (Remaining Time: 0)\n", running_process);
            terminate(ps[running_process]);

            waitpid(ps[running_process], NULL, 0);

            data[running_process].ct = total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;

            data[running_process].at = 10000000;

            running_process = -1;
        }
    }

    check_burst();

    ProcessData next_process = find_next_process();

    if (!(next_process.idx == running_process)) {
        running_process = next_process.idx;

        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);

        create_process(running_process);

        data[running_process].rt = total_time - data[running_process].at;
    }
}


int main(int argc, char *argv[]) {

    FILE *in_file = fopen(argv[1], "r");
    if (in_file == NULL) {
        printf("File is not found or cannot open it!\n");
        exit(EXIT_FAILURE);
    } else {
        read_file(in_file);
    }


    struct itimerval timer;
timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);

    signal(SIGALRM, schedule_handler);

    while (1); 
}
