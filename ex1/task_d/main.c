#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    // 8 GB (sizeof(long) = 8 bytes)
    long xy_size    = 1000*1000*1000;
    long x_dim      = 10000;
    long y_dim      = xy_size/x_dim;

    printf("Y-dim malloc: %li \n", y_dim*sizeof(long*));
    printf("x-dim malloc: %li \n", x_dim*sizeof(long));
    printf("Total GiB: %li \n",
            x_dim*sizeof(long)*y_dim*sizeof(long*)/8000000000);

    long** matrix   = malloc(y_dim*sizeof(long*));

    for(long y = 0; y < y_dim; y++){
        matrix[y] = malloc(x_dim*sizeof(long));
        memset(matrix[y], 0, x_dim*sizeof(long));
    }

    printf("Allocation complete (press any key to continue...)\n");
    getchar();
    return 0;
}
