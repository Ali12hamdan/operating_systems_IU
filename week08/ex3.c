#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

int main() {
    struct rusage usage;

    for (int i = 0; i < 10; i++) {

        char *buffer = (char *)malloc(10 * 1024 * 1024);

        if (buffer == NULL) {
            perror("Failed to allocate memory");
            return 1;
        }

        memset(buffer, 0, 10 * 1024 * 1024);

        getrusage(RUSAGE_SELF, &usage);
        printf("Memory usage: %ld kilobytes\n", usage.ru_maxrss);

        sleep(1);
    }

    return 0;
}
