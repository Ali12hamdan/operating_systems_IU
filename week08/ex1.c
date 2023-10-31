#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>

#ifndef MAP_ANONYMOUS
  #ifdef MAP_ANON
    #define MAP_ANONYMOUS MAP_ANON
  #else
    #define MAP_ANONYMOUS 0x20
  #endif
#endif

int main() {
    // Write the PID to /tmp/ex1.pid
    FILE *file = fopen("/tmp/ex1.pid", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    fprintf(file, "%d", getpid());
    fclose(file);

    // Generate random password
    char password[9];  // 8 characters + null terminator
    strcpy(password, "pass:");
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open /dev/urandom");
        return 1;
    }

    for (int i = 5; i < 8; i++) {
        unsigned char rand_char;
        do {
            read(fd, &rand_char, 1);
        } while (rand_char < 32 || rand_char > 126);  // Ensure it's a printable character
        password[i] = rand_char;
    }
    password[8] = '\0';  // Null terminate the string
    close(fd);

    // Store the password in memory using mmap
    char *addr = mmap(NULL, 9, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }
    strcpy(addr, password);

    // Wait in an infinite loop
    while (1) {
        sleep(1);
    }

    return 0;
}
