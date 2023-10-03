#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        printf("Enter a command (or 'exit' to quit): ");
        fgets(command, sizeof(command), stdin);

        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        if (strcmp(command, "exit") == 0) {
            break;
        }

        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            char *args[MAX_ARGS];
            int arg_count = 0;

            char *token = strtok(command, " ");
            while (token != NULL && arg_count < MAX_ARGS - 1) {
                args[arg_count] = token;
                token = strtok(NULL, " ");
                arg_count++;
            }
            args[arg_count] = NULL;

            int run_in_background = 0;
            if (arg_count > 0 && strcmp(args[arg_count - 1], "&") == 0) {
                run_in_background = 1;
                args[arg_count - 1] = NULL;
            }

            if (run_in_background) {
                execvp(args[0], args);
                //sleep(5);
                perror("execvp"); 
                exit(EXIT_FAILURE);
            } else {
                execvp(args[0], args);
                                //sleep(5);

                perror("execvp"); 
                exit(EXIT_FAILURE);
            }
            wait(NULL);
        }
    }

    return 0;
}
