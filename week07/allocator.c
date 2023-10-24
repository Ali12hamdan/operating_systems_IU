#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEMORY_SIZE 10000000
#define MAX_QUERIES 1000000

unsigned int memory[MEMORY_SIZE];

void reset_memory() {
    memset(memory, 0, sizeof(memory));
}

void first_fit(unsigned int adrs, unsigned int size) {
    int start = -1;
    int count = 0;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            if (start == -1) {
                start = i;
            }
            count++;
            if (count == size) {
                for (int j = start; j < start + size; j++) {
                    memory[j] = adrs;
                }
                return;
            }
        } else {
            start = -1;
            count = 0;
        }
    }
    // printf("Memory not available for allocation.\n");
}

void best_fit(unsigned int adrs, unsigned int size) {
    int start = -1;
    int count = 0;
    int best_start = -1;
    int best_count = MEMORY_SIZE + 1;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            if (start == -1) {
                start = i;
            }
            count++;
            if (count == size) {
                if (count < best_count) {
                    best_start = start;
                    best_count = count;
                }
            }
        } else {
            start = -1;
            count = 0;
        }
    }
    if (best_start != -1) {
        for (int j = best_start; j < best_start + size; j++) {
            memory[j] = adrs;
        }
     } //else {
    //     printf("Memory not available for allocation.\n");
    // }
}

void worst_fit(unsigned int adrs, unsigned int size) {
    int start = -1;
    int count = 0;
    int worst_start = -1;
    int worst_count = 0;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            if (start == -1) {
                start = i;
            }
            count++;
            if (count == size) {
                if (count > worst_count) {
                    worst_start = start;
                    worst_count = count;
                }
            }
        } else {
            start = -1;
            count = 0;
        }
    }
    if (worst_start != -1) {
        for (int j = worst_start; j < worst_start + size; j++) {
            memory[j] = adrs;
        }
     } 
}

void clear(unsigned int adrs) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == adrs) {
            memory[i] = 0;
        }
    }
}

void allocate(char *algo, unsigned int adrs, unsigned int size) {
    if (strcmp(algo, "first") == 0) {
        first_fit(adrs, size);
    } else if (strcmp(algo, "best") == 0) {
        best_fit(adrs, size);
    } else if (strcmp(algo, "worst") == 0) {
        worst_fit(adrs, size);
    } else {
        printf("Invalid algorithm\n");
    }
}

int main() {
    FILE *fp;
    char str[100];
    char action[20];
    char algo[20];
    unsigned int adrs, size;
    clock_t start, end;
    double cpu_time_used;
    int queries = 0;

    reset_memory();

    fp = fopen("queries.txt", "r");
    if (fp == NULL) {
        printf("File not found\n");
        return 0;
    }

    start = clock();

    while (fgets(str, 100, fp) != NULL) {
        sscanf(str, "%s", action);
        if (strcmp(action, "allocate") == 0) {
            sscanf(str, "%s %u %u", action, &adrs, &size);
            allocate("first", adrs, size);
        } else if (strcmp(action, "clear") == 0) {
            sscanf(str, "%s %u", action, &adrs);
            clear(adrs);
        } else if (strcmp(action, "end") == 0) {
            break;
        }
        queries++;
        if (queries >= MAX_QUERIES) {
            break;
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Throughput: %.2f queries per second\n", queries / cpu_time_used);

    fclose(fp);


    fp = fopen("queries.txt", "r");
    if (fp == NULL) {
        printf("File not found\n");
        return 0;
    }


    start = clock();

    while (fgets(str, 100, fp) != NULL) {
        sscanf(str, "%s", action);
        if (strcmp(action, "allocate") == 0) {
            sscanf(str, "%s %u %u", action, &adrs, &size);
            allocate("worst", adrs, size);
        } else if (strcmp(action, "clear") == 0) {
            sscanf(str, "%s %u", action, &adrs);
            clear(adrs);
        } else if (strcmp(action, "end") == 0) {
            break;
        }
        queries++;
        if (queries >= MAX_QUERIES) {
            break;
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Throughput: %.2f queries per second\n", queries / cpu_time_used);

    fclose(fp);


    fp = fopen("queries.txt", "r");
    if (fp == NULL) {
        printf("File not found\n");
        return 0;
    }
    


    start = clock();

    while (fgets(str, 100, fp) != NULL) {
        sscanf(str, "%s", action);
        if (strcmp(action, "allocate") == 0) {
            sscanf(str, "%s %u %u", action, &adrs, &size);
            allocate("best", adrs, size);
        } else if (strcmp(action, "clear") == 0) {
            sscanf(str, "%s %u", action, &adrs);
            clear(adrs);
        } else if (strcmp(action, "end") == 0) {
            break;
        }
        queries++;
        if (queries >= MAX_QUERIES) {
            break;
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Throughput: %.2f queries per second\n", queries / cpu_time_used);

    fclose(fp);

    
    return 0;
}
