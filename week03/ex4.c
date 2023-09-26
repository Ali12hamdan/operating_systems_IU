#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>


void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)) {
    void* result = initial_value;
    for (int i = 0; i < n; i++) {
        void* current = (char*)base + (i * size);
        result = opr(result, current);
    }
    return result;
}

void* addition(const void* a, const void* b) {
    int* result = malloc(sizeof(int));
    *result = *(int*)a + *(int*)b;
    return result;
}

void* multiplication(const void* a, const void* b) {
    int* result = malloc(sizeof(int));
    *result = *(int*)a * *(int*)b;
    return result;
}

void* max(const void* a, const void* b) {
    int* result = malloc(sizeof(int));
    *result = *(int*)a > *(int*)b ? *(int*)a : *(int*)b;
    return result;
}


void* addition_dou(const void* a, const void* b) {
    double* result = malloc(sizeof(double));
    *result = *(double*)a + *(double*)b;
    return result;
}

void* multiplication_dou(const void* a, const void* b) {
    double* result = malloc(sizeof(double));
    *result = *(double*)a * *(double*)b;
    return result;
}

void* max_dou(const void* a, const void* b) {
    double* result = malloc(sizeof(double));
    *result = *(double*)a > *(double*)b ? *(double*)a : *(double*)b;
    return result;
}


int main() {
    double doubleArray[5] = {1.5, 2.5, 3.5, 4.5, 5.5};
    int intArray[5] = {1, 2, 3, 4, 5};

    int var1 = 0;
    int var2 = 1;
    int var3 = INT_MIN;
    
    double vard1 = 0.0;
    double vard2 = 1.0;
    double vard3 = DBL_MIN;






    double* doubleSum = aggregate(doubleArray, sizeof(double), 5, &vard1, addition_dou);
    printf("Sum of doubleArray: %.2f\n", *doubleSum);
    free(doubleSum);

    double* doublemult = aggregate(doubleArray, sizeof(double), 5, &vard2, multiplication_dou);
    printf("Sum of doubleArray: %.2f\n", *doubleSum);
    free(doublemult);

    double* doubleMax = aggregate(doubleArray, sizeof(double), 5, &vard3, max_dou);
    printf("Sum of doubleArray: %.2f\n", *doubleSum);
    free(doubleMax);

    int* intProduct = aggregate(intArray, sizeof(int), 5, &var2, multiplication);
    printf("Product of intArray: %d\n", *intProduct);
    free(intProduct);

    int* intMax = aggregate(intArray, sizeof(int), 5, &var3 , max);
    printf("Max of intArray: %d\n", *intMax);
    free(intMax);
        int* intSum = aggregate(intArray, sizeof(int), 5, &var1 , addition);
    printf("Max of intArray: %d\n", *intMax);
    free(intSum);

    return 0;
}
