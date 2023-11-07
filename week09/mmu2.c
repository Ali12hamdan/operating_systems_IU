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
    unsigned char counter;  // for Aging
} PTE;

struct TLB_entry {
    int page;
    int frame;
};

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <number_of_pages> <reference_string> <pager_pid> <algorithm>\n", argv[0]);
        exit(1);
    }

    int num_pages = atoi(argv[1]);
    char *ref_string = argv[2];
    pid_t pager_pid = (pid_t) atoi(argv[3]);
    char *algorithm = argv[4];

    printf("Selected algorithm: %s\n", algorithm);

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

    int TLB_size = num_pages * 0.2;
    struct TLB_entry *TLB = malloc(TLB_size * sizeof(struct TLB_entry));

    int hits = 0, misses = 0, TLB_lookups = 0, TLB_misses = 0;

    for (int i = 0; i < strlen(ref_string); i += 2) {
        char mode = ref_string[i];
        int page = ref_string[i + 1] - '0';

        TLB_lookups++;
        int frame = -1;
        for (int j = 0; j < TLB_size; j++) {
            if (TLB[j].page == page) {
                frame = TLB[j].frame;
                break;
            }
        }

        if (frame == -1) {  // TLB miss
            TLB_misses++;
            // Check the page table and update the TLB...
        }

        if (page_table[page].valid) {
            hits++;
        } else {
            misses++;
        }

        printf("Mode: %c, Page: %d\n", mode, page);
    }

    printf("Hit ratio: %.2f\n", (float)hits / (hits + misses));
    printf("TLB miss ratio: %.2f\n", (float)TLB_misses / TLB_lookups);

    free(TLB);
    munmap(page_table, num_pages * sizeof(PTE));
    close(fd);

    kill(pager_pid, SIGUSR1);

    return 0;
}
