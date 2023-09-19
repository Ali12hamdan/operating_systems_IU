#include <stdio.h>

void convert(long long num, int s, int t) {
    if (s < 2 || s > 10 || t < 2 || t > 10) {
        printf("Cannot convert!\n");
        return;
    }

    long long deci = 0;
    long long multiplier = 1;
    while (num > 0) {
        int digit = num % 10;
        if (digit >= s) {
            printf("Cannot convert!\n");
            return;
        }
        deci += digit * multiplier;
        multiplier *= s;
        num /= 10;
    }

    long long converted = 0;
    long long placeValue = 1;
    while (deci > 0) {
        int remainder = deci % t;
        converted += remainder * placeValue;
        placeValue *= 10;
        deci /= t;
    }

    printf("%lld\n", converted);
}

int main() {
    long long num;
    int sourceBase, targetBase;

    printf("Enter a number: ");
    scanf("%lld", &num);

    printf("Enter origin base (2-10): ");
    scanf("%d", &sourceBase);

    printf("Enter wanted base (2-10): ");
    scanf("%d", &targetBase);

    convert(num, sourceBase, targetBase);

    return 0;
}
