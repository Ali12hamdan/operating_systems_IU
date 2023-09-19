#include <stdio.h>

int tribonacci(int n) {
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;

    int a = 0, b = 1, c = 1, nextVal;

    for (int i = 3; i <= n; i++) {
        nextVal = a + b + c;
        a = b;
        b = c;
        c = nextVal;
    }

    return c;
}

int main() {
    int n1 = 4;
    int n2 = 36;

    int result_1 = tribonacci(n1);
    int result_2 = tribonacci(n2);

    printf("Tribonacci(%d) = %d\n", n1, result_1);
    printf("Tribonacci(%d) = %d\n", n2, result_2);

    return 0;
}
