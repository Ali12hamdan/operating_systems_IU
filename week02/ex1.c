#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
    int integerVar;
    unsigned short int ushortVar;
    long int longVar;
    float floatVar;
    double doubleVar;

    integerVar = INT_MAX;
    ushortVar = USHRT_MAX;
    longVar = LONG_MAX;
    floatVar = FLT_MAX;
    doubleVar = DBL_MAX;

    printf("integerVariable size: %lu bytes\n", sizeof(integerVar));
    printf("Max integerVariable value: %d\n", integerVar);

    printf("UnshortVariable size: %lu bytes\n", sizeof(ushortVar));
    printf("Max UnshortVariable value: %u\n", ushortVar);

    printf("longVariable size: %lu bytes\n", sizeof(longVar));
    printf("Max longVariable value: %ld\n", longVar);

    printf("FloatVariable size: %lu bytes\n", sizeof(floatVar));
    printf("Max floatVariable value: %f\n", floatVar);

    printf("DoubleVariable size: %lu bytes\n", sizeof(doubleVar));
    printf("Max doubleVariable value: %lf\n", doubleVar);

    return 0;
}