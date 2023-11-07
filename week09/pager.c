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

PTE *page_table;
int num_pages;
int disk_accesses = 0;

void handle_signal(int sig) {
    if (sig == SIGUSR1) {
        printf("Received SIGUSR1 from MMU.\n");
        disk_accesses++;
    }
}

int random(PTE* page_table, int num_pages) {
    return rand() % num_pages;
}

int nfu(PTE* page_table, int num_pages) {
    int min_index = 0;
    for (int i = 1; i < num_pages; i++) {
        if (page_table[i].referenced < page_table[min_index].referenced) {
            min_index = i;
        }
    }
    return min_index;
}

int aging(PTE* page_table, int num_pages) {
    int min_index = 0;
    for (int i = 1; i < num_pages; i++) {
        if (page_table[i].counter < page_table[min_index].counter) {
            min_index = i;
        }
    }
    return min_index;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <number_of_pages> <mmu_pid> <algorithm>\n", argv[0]);
        exit(1);
    }

    num_pages = atoi(argv[1]);
    pid_t mmu_pid = (pid_t) atoi(argv[2]);
    char *algorithm = argv[3];

    signal(SIGUSR1, handle_signal);

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
        page_table[i].counter = 0;  // for Aging
    }

    while (1) {
        pause();
        int page_to_evict;
        if (strcmp(algorithm, "random") == 0) {
            page_to_evict = random(page_table, num_pages);
        } else if (strcmp(algorithm, "nfu") == 0) {
            page_to_evict = nfu(page_table, num_pages);
        } else if (strcmp(algorithm, "aging") == 0) {
            page_to_evict = aging(page_table, num_pages);
        } else {
            fprintf(stderr, "Invalid algorithm: %s\n", algorithm);
            exit(1);
        }
        // Evict the selected page
        page_table[page_to_evict].valid = 0;
    }

    printf("Total disk accesses: %d\n", disk_accesses);
    munmap(page_table, num_pages * sizeof(PTE));
    close(fd);
    remove("/tmp/ex2/pagetable");
    return 0;
}
