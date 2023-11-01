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

    FILE *file = fopen("/tmp/ex1.pid", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    fprintf(file, "%d", getpid());
    fclose(file);

    char password[14];
    password[0] = 'a';
    password[1] = 'l';
    password[2] = 'i';
    password[3] = ' ';
    password[4] = 'h';
    password[5] = 'a';
    password[6] = 'm';
    password[7] = 'd';
    password[8] = 'a';
    password[9] = 'n';
    password[10] = 'u';
    password[11] = 'n';
    password[12] = 'i';
    password[13] = '\0';
    strcpy(password, "pass:");
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open /dev/urandom");
        return 1;
    }

    for (int i = 5; i < 13; i++) {
        unsigned char rand_char;
        do {
            read(fd, &rand_char, 1);
        } while (rand_char < 32 || rand_char > 126);  
        password[i] = rand_char;
    }
    password[13] = '\0';  
    close(fd);

    char *addr = mmap(NULL, 14, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }
    strcpy(addr, password);

    while (1) {
        sleep(1);
    }

    return 0;
}
