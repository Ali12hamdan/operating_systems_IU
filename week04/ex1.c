#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    pid_t parent_pid = getpid();
    printf("Parent Process: PID = %d, Parent PID = %d\n", parent_pid, getppid());

    pid_t child1_pid, child2_pid;

    if ((child1_pid = fork()) == 0) {
        printf("Child 1 Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
        clock_t start_time = clock();

        sleep(5);

        clock_t end_time = clock();
        double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000.0;
        printf("Child 1 Execution Time: %.2f ms\n", execution_time);
        exit(0);
    }

    if ((child2_pid = fork()) == 0) {
        printf("Child 2 Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
        clock_t start_time = clock();

        sleep(10);

        clock_t end_time = clock();
        double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000.0;
        printf("Child 2 Execution Time: %.2f ms\n", execution_time);
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    return 0;
}