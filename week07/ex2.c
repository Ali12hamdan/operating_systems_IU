#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

int main() {
    // Creating an empty file
    FILE *file = fopen("text.txt", "w");
    if (file == NULL) {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }
    fclose(file);

    // Open /dev/random
    int randomFile = open("/dev/random", O_RDONLY);
    if (randomFile == -1) {
        perror("Error opening /dev/random");
        exit(EXIT_FAILURE);
    }

    // Open text.txt
    int textFile = open("text.txt", O_RDWR);
    if (textFile == -1) {
        perror("Error opening text.txt");
        exit(EXIT_FAILURE);
    }

    // Seek to the end of the file
    if (lseek(textFile, 0, SEEK_END) == -1) {
        perror("Error seeking to the end of the file");
        exit(EXIT_FAILURE);
    }

    // Memory map the text file
    struct stat st;
    if (fstat(textFile, &st) == -1) {
        perror("Error getting file status");
        exit(EXIT_FAILURE);
    }

    long page_size = sysconf(_SC_PAGESIZE);
    long chunk_size = 1024 * page_size;

    void *mapped = mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, textFile, 0);
    if (mapped == MAP_FAILED) {
        close(textFile);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }

    // Generate and add characters
    long bytes_written = st.st_size;
    while (bytes_written < 500 * 1024 * 1024) {
        char c;
        read(randomFile, &c, 1);
        if (isprint(c) && bytes_written % (1024 * sizeof(char) + 1) != 1024 * sizeof(char)) {
            *((char *)mapped + bytes_written) = c;
            bytes_written++;
        }
    }

    // Unmap the memory
    if (munmap(mapped, st.st_size) == -1) {
        perror("Error un-mmapping the file");
    }
    close(textFile);
    close(randomFile);

    // Open the text file again for reading and processing chunks
    textFile = open("text.txt", O_RDWR);
    if (textFile == -1) {
        perror("Error opening text.txt again");
        exit(EXIT_FAILURE);
    }

    mapped = mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, textFile, 0);
    if (mapped == MAP_FAILED) {
        close(textFile);
        perror("Error mmapping the file again");
        exit(EXIT_FAILURE);
    }

    // Count the capital letters and replace them with lowercase letters
    int capital_count = 0;
    for (long i = 0; i < st.st_size; i++) {
        char *current_char = (char *)mapped + i;
        if (*current_char >= 'A' && *current_char <= 'Z') {
            capital_count++;
            *current_char = tolower(*current_char);
        }
    }
    printf("Total number of capital letters: %d\n", capital_count);

    // Unmap the memory again
    if (munmap(mapped, st.st_size) == -1) {
        perror("Error un-mmapping the file again");
    }
    close(textFile);

    return 0;
}
