#include <stdio.h>
#include <math.h>

struct Point {
    double x;
    double y;
};

double distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

double area(struct Point A, struct Point B, struct Point C) {
    return 0.5 * fabs(A.x * B.y - A.y * B.x + B.x * C.y - B.y * C.x + C.x * A.y - C.y * A.x);
}

int main() {
    struct Point A = {2.5, 6};
    struct Point B = {1, 2.2};
    struct Point C = {10, 6};

    double AB_distance = distance(A, B);
    double ABC_area = area(A, B, C);

    printf("Distance between A and B: %lf\n", AB_distance);
    printf("Area of the triangle ABC: %lf\n", ABC_area);

    return 0;
}
