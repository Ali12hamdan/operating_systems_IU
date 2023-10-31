#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    int valid;
    int dirty;
    int frame;
    int referenced;
} PTE;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <number_of_pages> <reference_string> <pager_pid>\n", argv[0]);
        exit(1);
    }

    int num_pages = atoi(argv[1]);
    char *ref_string = argv[2];
    pid_t pager_pid = (pid_t) atoi(argv[3]);

    // Open the mapped file
    int fd = open("/tmp/ex2/pagetable", O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    PTE *page_table = mmap(NULL, num_pages * sizeof(PTE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        exit(1);
    }

    // Iterate over the reference string
    for (int i = 0; i < strlen(ref_string); i += 2) {
        char mode = ref_string[i];
        int page = ref_string[i + 1] - '0';
        
        // Implement MMU logic based on the instructions...

        // Placeholder for checking if the page is in RAM, handling page faults, etc.
        printf("Mode: %c, Page: %d\n", mode, page);
    }

    // Close and unmap
    munmap(page_table, num_pages * sizeof(PTE));
    close(fd);

    // Signal the pager
    kill(pager_pid, SIGUSR1);

    return 0;
}
