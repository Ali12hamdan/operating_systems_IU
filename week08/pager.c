#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define PAGE_SIZE 8 // Define the size of each page/frame

typedef struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
} PTE;

// Globals
char **RAM = NULL;
char **disk = NULL;
int frames, pages;

// Function prototypes
void initialize_disk(int pages);
void print_ram(int frames);
void print_disk(int pages);
void simulate_page_request(PTE *page_table, int page_number, pid_t mmu_pid);
void pager_load_page_to_ram(int page_number, int frame_number);
void pager_write_page_to_disk(int page_number, int frame_number);
void cleanup();

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_frames> <number_of_pages>\n", argv[0]);
        return EXIT_FAILURE;
    }

    frames = atoi(argv[1]);
    pages = atoi(argv[2]);

    // Allocate memory for RAM
    RAM = (char **)malloc(frames * sizeof(char *));
    if (RAM == NULL) {
        perror("Memory allocation for RAM failed");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < frames; i++) {
        RAM[i] = (char *)malloc((PAGE_SIZE + 1) * sizeof(char));
        if (RAM[i] == NULL) {
            perror("Memory allocation for RAM frame failed");
            cleanup();
            return EXIT_FAILURE;
        }
        memset(RAM[i], 0, PAGE_SIZE + 1); // Initialize with zeroes
    }

    // Allocate memory for disk
    disk = (char **)malloc(pages * sizeof(char *));
    if (disk == NULL) {
        perror("Memory allocation for disk failed");
        cleanup();
        return EXIT_FAILURE;
    }
    for (int i = 0; i < pages; i++) {
        disk[i] = (char *)malloc((PAGE_SIZE + 1) * sizeof(char));
        if (disk[i] == NULL) {
            perror("Memory allocation for disk page failed");
            cleanup();
            return EXIT_FAILURE;
        }
    }

    // Initialize the disk with random messages
    initialize_disk(pages);
    print_disk(pages);

    pid_t mmu_pid = getpid();
    PTE *page_table;
    size_t page_table_size = pages * sizeof(PTE);
    int fd;

    // Open or create the page table file
    fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        cleanup();
        return EXIT_FAILURE;
    }

    // Ensure the file is the correct size
    if (ftruncate(fd, page_table_size) == -1) {
        perror("Error setting file size");
        close(fd);
        cleanup();
        return EXIT_FAILURE;
    }

    // Map the file to memory
    page_table = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        cleanup();
        return EXIT_FAILURE;
    }

    // Initialize the page table entries to invalid
    for (int i = 0; i < pages; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }

    // Simulate page requests
    for (int i = 0; i < pages; i++) {
        simulate_page_request(page_table, i, mmu_pid);
        print_ram(frames);
    }

    // Clean up
    munmap(page_table, page_table_size);
    close(fd);
    cleanup();

    return 0;
}

void initialize_disk(int pages) {
    srand(time(NULL));
    for (int i = 0; i < pages; i++) {
        for (int j = 0; j < PAGE_SIZE; j++) {
            disk[i][j] = ' ' + (rand() % ('~' - ' ')); // Random printable character
        }
        disk[i][PAGE_SIZE] = '\0'; // Null-terminate the string
    }
}

void print_ram(int frames) {
    printf("RAM state:\n");
    for (int i = 0; i < frames; i++) {
        printf("%d: \"%s\"\n", i, RAM[i]);
    }
}

void print_disk(int pages) {
    printf("Disk state:\n");
    for (int i = 0; i < pages; i++) {
        printf("%d: \"%s\"\n", i, disk[i]);
    }
}

void simulate_page_request(PTE *page_table, int page_number, pid_t mmu_pid) {
    if (!page_table[page_number].valid) {
        // Set the referenced field to the PID of the MMU
        page_table[page_number].referenced = (int)mmu_pid;
        // Simulate a page fault
        kill(mmu_pid, SIGUSR1);
        pause(); // Wait for the SIGCONT from the pager
    }
    // Here, after receiving SIGCONT, we would handle the page request.
    // This is where you would add code to modify the page table to reflect
    // the new state of the page in memory.
}

void pager_load_page_to_ram(int page_number, int frame_number) {
    // Copy the disk page to RAM
    strncpy(RAM[frame_number], disk[page_number], PAGE_SIZE);
}

void pager_write_page_to_disk(int page_number, int frame_number) {
    // Copy the RAM frame to disk
    strncpy(disk[page_number], RAM[frame_number], PAGE_SIZE);
}

void cleanup() {
    // Free RAM memory
    for (int i = 0; i < frames; i++) {
        free(RAM[i]);
    }
    free(RAM);

    // Free disk memory
    for (int i = 0; i < pages; i++) {
        free(disk[i]);
    }
    free(disk);
}
