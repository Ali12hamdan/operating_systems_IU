#include <stdio.h>
#include <stdlib.h>

const int const_tri(int* p, int n) {
    int temp;
    p[0] = 0;
    p[1] = p[2] = 1;
    if (n == 0){
        return p[0];

    }
    else if (n == 1 || n == 2){
        return p[1];

    }

    for(int i = 3; i <= n; i++){
        temp = p[0] + p[1] + p[2];
        p[0] = p[1];
        p[1] = p[2];
        p[2] = temp;
    }
    return p[2];

}



int main() {
    const int x = 1;
    int* const p = (int*)malloc(3 * sizeof(int));
    if(p == NULL){
        printf("FAILED to allocate memory.\n");
    }

    int const* q = &x;
    p[0] = *q;
    p[1] = *q;
    p[2] = 2 * *q;

    printf("Addresses for the memory:\n");
    for (int i = 0; i < 3; ++i) {
        printf("Cell no %d: %p\n", i, (void*)&p[i]);
    }

    if (&p[1] - &p[0] == 1 && &p[2] - &p[1] == 1) {
        printf("Contiguous Cells.\n");
    } else {
        printf("Contiguous Cells.\n");
    }
    printf("%d\n", const_tri(p,4));

    
    free(p); 
    return 0;}

