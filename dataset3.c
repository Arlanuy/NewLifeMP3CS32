#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int i, j;
    FILE* fp = fopen("dataset3.txt", "a");
    int** edge_cost_mat = malloc(12* sizeof(int*)); 
    for (i = 1; i <= 12; i++) {
        edge_cost_mat[i - 1] = malloc(12 * sizeof(int));
        for (j = 1; j <= 12; j++) {
            if (i == j) {
                edge_cost_mat[i - 1][j - 1] = 0;
            }
            
            else {
                edge_cost_mat[i - 1][j - 1] = (22 * i) + j;
            }
            if (j != 12) {
                fprintf(fp, "%d ", edge_cost_mat[i - 1][j - 1]);    
            }
            else {
                fprintf(fp, "%d\n", edge_cost_mat[i - 1][j - 1]); 
            }
        }

    }
}