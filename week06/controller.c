#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define FILE_PATH "/var/run/agent.pid"

void sigint_handler(int signo) {

    if (signo == SIGINT) {
        FILE *fp = fopen(FILE_PATH, "r");
        if (!fp) {

            perror("Error opening file");
            exit(1);
        }

        int pid;
        fscanf(fp, "%d", &pid);
        fclose(fp);
        kill(pid, SIGTERM);
        exit(0);
    }
}




int main() {

    FILE *fp = fopen(FILE_PATH, "r");

    if (!fp) {
        printf("Error: No agent found.\n");
        exit(1);
    }

    printf("Agent found.\n");
    fclose(fp);

    signal(SIGINT, sigint_handler);

    char command[10];

    while (1) {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"} to send to the agent: ");
        scanf("%s", command);

        if (strcmp(command, "read") == 0) {
            FILE *fp = fopen(FILE_PATH, "r");
            if (!fp) {
                perror("Error opening file");
                exit(1);
            }
            int pid;
            fscanf(fp, "%d", &pid);
            fclose(fp);
            kill(pid, SIGUSR1);

        } else if (strcmp(command, "exit") == 0) {
            FILE *fp = fopen(FILE_PATH, "r");

            if (!fp) {
            
                perror("Error opening file");
                exit(1);
            }
            int pid;
            fscanf(fp, "%d", &pid);
            fclose(fp);
            kill(pid, SIGUSR2);
            exit(0);

        } else if (strcmp(command, "stop") == 0) {
            FILE *fp = fopen(FILE_PATH, "r");
            if (!fp) {
                perror("Error opening file");
                exit(1);
            }
            int pid;
            fscanf(fp, "%d", &pid);
            fclose(fp);
            kill(pid, SIGSTOP);
        } 
        
        else if (strcmp(command, "continue") == 0) {
            FILE *fp = fopen(FILE_PATH, "r");
            if (!fp) {
                perror("Error opening file");
                exit(1);
            }
            int pid;
            fscanf(fp, "%d", &pid);
            fclose(fp);
            kill(pid, SIGCONT);
        } 
        
        else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}
