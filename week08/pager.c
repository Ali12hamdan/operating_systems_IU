#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {
    int valid;
    int dirty;
    int frame;
    int referenced;
} PTE;

// Global variables
PTE *page_table;
int num_pages;
int disk_accesses = 0;

void handle_signal(int sig) {
    // Placeholder for handling signals from MMU
    if (sig == SIGUSR1) {
        // Implement logic for handling page faults, updating page table, etc.
        printf("Received SIGUSR1 from MMU.\n");
        disk_accesses++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_pages> <mmu_pid>\n", argv[0]);
        exit(1);
    }

    num_pages = atoi(argv[1]);
    pid_t mmu_pid = (pid_t) atoi(argv[2]);

    // Set up signal handler
    signal(SIGUSR1, handle_signal);

    // Create and initialize the page table
    int fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }
    ftruncate(fd, num_pages * sizeof(PTE));
    page_table = mmap(NULL, num_pages * sizeof(PTE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        exit(1);
    }
    for (int i = 0; i < num_pages; i++) {
        page_table[i].valid = 0;
        page_table[i].dirty = 0;
        page_table[i].frame = -1;
        page_table[i].referenced = 0;
    }

    // Placeholder for main loop to handle signals and MMU requests
    while (1) {
        pause();  // Wait for signals
    }

    // Cleanup and exit
    printf("Total disk accesses: %d\n", disk_accesses);
    munmap(page_table, num_pages * sizeof(PTE));
    close(fd);
    remove("/tmp/ex2/pagetable");
    return 0;
}
