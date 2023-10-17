#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define FILE_PATH "/var/run/agent.pid"
#define TEXT_FILE_PATH "text.txt"

void sig_handler(int signo) {

    if (signo == SIGUSR1) {
        FILE *file = fopen(TEXT_FILE_PATH, "r");

        if (file == NULL) {
            perror("Error opening file");
            exit(1);
        }

        char buffer[255];
        while (fgets(buffer, sizeof(buffer), file)) {
            printf("%s", buffer);
        }

        fclose(file);

    }
    else if (signo == SIGUSR2) {
        printf("Process terminating...\n");
        exit(0);
    }
}




int main() {

    FILE *fp = fopen(FILE_PATH, "w");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    fprintf(fp, "%d", getpid());
    fclose(fp);

    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);



    FILE *text_file = fopen(TEXT_FILE_PATH, "r");

    if (!text_file) {
        perror("Error opening file");
        return 1;
    }

    char buffer[255];
    
    while (fgets(buffer, sizeof(buffer), text_file)) {
        printf("%s", buffer);
    }

    fclose(text_file);

    while (1) {
        sleep(1);
    }

    return 0;
}
