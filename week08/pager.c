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

#define PAGE_SIZE 8 

typedef struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
} PTE;

char **RAM = NULL;
char **disk = NULL;
int frames, pages;

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
        memset(RAM[i], 0, PAGE_SIZE + 1); 
    }

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

    initialize_disk(pages);
    print_disk(pages);

    pid_t mmu_pid = getpid();
    PTE *page_table;
    size_t page_table_size = pages * sizeof(PTE);
    int fd;

    fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        cleanup();
        return EXIT_FAILURE;
    }

    if (ftruncate(fd, page_table_size) == -1) {
        perror("Error setting file size");
        close(fd);
        cleanup();
        return EXIT_FAILURE;
    }

    page_table = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        cleanup();
        return EXIT_FAILURE;
    }

    for (int i = 0; i < pages; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }

    for (int i = 0; i < pages; i++) {
        simulate_page_request(page_table, i, mmu_pid);
        print_ram(frames);
    }

    munmap(page_table, page_table_size);
    close(fd);
    cleanup();

    return 0;
}

void initialize_disk(int pages) {
    srand(time(NULL));
    for (int i = 0; i < pages; i++) {
        for (int j = 0; j < PAGE_SIZE; j++) {
            disk[i][j] = ' ' + (rand() % ('~' - ' ')); 
        }
        disk[i][PAGE_SIZE] = '\0'; 
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
        page_table[page_number].referenced = (int)mmu_pid;
        kill(mmu_pid, SIGUSR1);
        pause(); 
    }
    
}

void pager_load_page_to_ram(int page_number, int frame_number) {
    strncpy(RAM[frame_number], disk[page_number], PAGE_SIZE);
}

void pager_write_page_to_disk(int page_number, int frame_number) {
    strncpy(disk[page_number], RAM[frame_number], PAGE_SIZE);
}

void cleanup() {
    for (int i = 0; i < frames; i++) {
        free(RAM[i]);
    }
    free(RAM);

    for (int i = 0; i < pages; i++) {
        free(disk[i]);
    }
    free(disk);
}
