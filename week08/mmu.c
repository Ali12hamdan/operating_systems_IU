#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

volatile sig_atomic_t page_loaded = 0;

void sigcont_handler(int sig) {
    page_loaded = 1;
}

void simulate_page_fault(int pager_pid) {
    kill(pager_pid, SIGUSR1);
    pause(); // Wait until SIGCONT is received
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <num_pages> <reference_string> <pager_pid>\n", argv[0]);
        return 1;
    }

    int num_pages = atoi(argv[1]);
    char* reference_string = argv[2];
    int pager_pid = atoi(argv[3]);
    struct PTE* page_table;
    int fd;
    size_t pt_size = num_pages * sizeof(struct PTE);

    // Set up signal handling for SIGCONT
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigcont_handler;
    sigaction(SIGCONT, &sa, NULL);

    // Open and map the page table file
    fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(fd, pt_size);
    page_table = mmap(NULL, pt_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    // Initialize the page table
    for (int i = 0; i < num_pages; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }

    printf("-------------------------\n");
    printf("Initialized page table\n");
    printf("Page table\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
               i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
    printf("-------------------------\n");

    // Process the reference string
    for (int i = 0; reference_string[i] != '\0'; i += 2) {
        char mode = reference_string[i];
        int page = reference_string[i+1] - '0';

        printf("%s Request for page %d\n", (mode == 'R' ? "Read" : "Write"), page);
        if (!page_table[page].valid) {
            printf("It is not a valid page --> page fault\n");
            printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
            page_table[page].referenced = getpid();
            simulate_page_fault(pager_pid);
            while (!page_loaded);
            page_loaded = 0;
            page_table[page].valid = true;
            page_table[page].frame = page; // For simplicity, let's assume frame number is same as page number
        }

        if (mode == 'W') {
            printf("It is a write request then set the dirty field\n");
            page_table[page].dirty = true;
        }

        // Print the page table
        printf("Page table\n");
        for (int j = 0; j < num_pages; j++) {
            printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
                   j, page_table[j].valid, page_table[j].frame, page_table[j].dirty, page_table[j].referenced);
        }
        printf("-------------------------\n");
    }

    // Signal the pager process one last time
    kill(pager_pid, SIGUSR1);
    munmap(page_table, pt_size);

    printf("Done all requests.\n");
    printf("MMU sends SIGUSR1 to the pager.\n");
    printf("MMU terminates.\n");

    return 0;
}
