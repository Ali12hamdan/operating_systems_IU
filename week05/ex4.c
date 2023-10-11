#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

bool is_prime(int n){
    if (n <= 1) return false;
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;
int k = 0;
int c = 0;
int n = 0;

int get_number_to_check() {
    int ret;
    pthread_mutex_lock(&global_lock);
    if (k != n) {
        ret = k;
        k++;
    } else {
        ret = -1;
    }
    pthread_mutex_unlock(&global_lock);
    return ret;
}

void increment_primes() {
    pthread_mutex_lock(&global_lock);
    c++;
    pthread_mutex_unlock(&global_lock);
}

void* check_primes(void* arg) {
    while (1) {
        int number = get_number_to_check();
        if (number == -1) {
            break; 
        }
        
        if (is_prime(number)) {
            increment_primes();
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s n m\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];
    
    for (int i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, check_primes, NULL);
    }

    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Total primes in [0, %d) = %d\n", n, c);

    return 0;
}
