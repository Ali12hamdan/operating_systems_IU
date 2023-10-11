#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

struct Thread {
    pthread_t id;
    int i;
    char message[256];
};

void *thread_function(void *arg) {
    struct Thread *thread_data = (struct Thread *)arg;
    
    printf("Thread %d is created.The ID is:%ld\n", thread_data->i,thread_data->id);
    
    snprintf(thread_data->message, sizeof(thread_data->message), "Hello from thread %d", thread_data->i);

    printf("%s\n", thread_data->message);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    if (n <= 0) {
        printf("Number of threads must be a positive integer.\n");
        return 1;
    }

    struct Thread threads[n];

    for (int i = 0; i < n; i++) {
        threads[i].i = i + 1;
        pthread_create(&threads[i].id, NULL, thread_function, &threads[i]);
        pthread_join(threads[i].id, NULL);
    }

    return 0;
}
