#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *p = NULL;

    p = (int *)malloc(10 * sizeof(int));

    if (p == NULL) {
        fprintf(stderr, "Error: malloc failed\n");
        return 1;
    }

    p[0] = 5;
    printf("%d\n", p[0]);

    p = (int *)malloc(20 * sizeof(int));
    p[0] = 10;
    printf("%d\n", p[0]);

    free(p);
    
    return 0;
}