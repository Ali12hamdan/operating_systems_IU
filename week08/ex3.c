#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

int main() {
    struct rusage usage;

    for (int i = 0; i < 10; i++) {
        // Allocate 10MB of memory
        char *buffer = (char *)malloc(10 * 1024 * 1024);

        if (buffer == NULL) {
            perror("Failed to allocate memory");
            return 1;
        }

        // Fill it with zeros
        memset(buffer, 0, 10 * 1024 * 1024);

        // Print memory usage using getrusage
        getrusage(RUSAGE_SELF, &usage);
        printf("Memory usage: %ld kilobytes\n", usage.ru_maxrss);

        // Sleep for 1 second
        sleep(1);
    }

    return 0;
}
