#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>

bool is_prime(int n) {
    if (n <= 1) return false;
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

typedef struct prime_request {
    int a, b;
    int count;
} prime_request;

void* prime_counter(void* arg) {
    prime_request* req = (prime_request*)arg;
    req->count = 0;
    
    for (int i = req->a; i < req->b; i++) {
        if (is_prime(i)) {
            req->count++;
        }
    }
    
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s n m\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];
    prime_request requests[m];

    int interval = n / m;
    int a = 0;
    int b = interval;

    for (int i = 0; i < m; i++) {
        requests[i].a = a;
        requests[i].b = (i == m - 1) ? n : b;
        pthread_create(&threads[i], NULL, prime_counter, &requests[i]);
        a = b;
        b += interval;
    }

    int total_primes = 0;
    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
        total_primes += requests[i].count;
    }

    printf("Total primes in [0, %d) = %d\n", n, total_primes);

    return 0;
}
