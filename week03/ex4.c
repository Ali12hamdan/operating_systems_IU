#include <stdio.h>
#include <stdlib.h>

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

int main() {
    double doubleArray[5] = {1.5, 2.5, 3.5, 4.5, 5.5};
    int intArray[5] = {1, 2, 3, 4, 5};

    int var1 = 5;
    float var2 = 0.0;



    double* doubleSum = aggregate(doubleArray, sizeof(double), 5, 0.0, addition);
    printf("Sum of doubleArray: %.2f\n", *doubleSum);
    free(doubleSum);

    int* intProduct = aggregate(intArray, sizeof(int), 5, 1, multiplication);
    printf("Product of intArray: %d\n", *intProduct);
    free(intProduct);

    int* intMax = aggregate(intArray, sizeof(int), 5, INT_MIN , max);
    printf("Max of intArray: %d\n", *intMax);
    free(intMax);

    return 0;
}
