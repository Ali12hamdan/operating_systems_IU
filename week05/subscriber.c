#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_MESSAGE_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error: No argument provided!\n");
        exit(EXIT_FAILURE);
    }

    int subscriber_id = atoi(argv[1]);
    char pipe_name[20];

    snprintf(pipe_name, sizeof(pipe_name), "/tmp/ex1/s%d_pipe", subscriber_id);
    int fd = open(pipe_name, O_RDONLY);

    if (fd == -1) {
        perror("Failed to open named pipe");
        printf("Pipe name: %s\n", pipe_name);
        return 1;
    }

    char message[MAX_MESSAGE_SIZE];
    ssize_t bytes_read;

    printf("Subscriber Process:\n");
    while ((bytes_read = read(fd, message, MAX_MESSAGE_SIZE)) > 0) {
        printf("%.*s", (int)bytes_read, message);
    }

    close(fd);

    return 0;
}
