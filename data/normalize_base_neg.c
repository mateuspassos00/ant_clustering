#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SAMPLE_SIZE 600

float normalize(float z, float z_cap) {
    if(z > 0) return z / z_cap;
    else return - z / z_cap;
}

int main() {

    FILE *fp = fopen64("base_15_grupos.csv", "rt"); assert(fp != NULL);    

    float x, y, x_min, x_max, y_min, y_max;
    int l;
    
    fscanf(fp, "%f,%f,%d\n", &x, &y, &l);
    x_max = x;
    x_min = x;
    y_max = y;
    y_min = y;
    
    for(int i = 1; i < SAMPLE_SIZE; i++) {
        fscanf(fp, "%f,%f,%d\n", &x, &y, &l);
        if(x < x_min) x_min = x;
        else if(x > x_max) x_max = x;
        
        if(y < y_min) y_min = y;
        else if(y > y_max) y_max = y;
    }

    printf("\nx_min = %f\nx_max = %f\ny_min = %f\ny_max = %f\n", x_min, x_max, y_min, y_max);

    fseek(fp, 0, 0);

    FILE *out = fopen64("base_15_grupos_norm_neg.csv", "wt"); assert(out != NULL);

    for(int i = 0; i < SAMPLE_SIZE; i++) {
        fscanf(fp, "%f,%f,%d\n", &x, &y, &l);
        switch (l) {
        case 1:
            fprintf(out, "%.6f,%.6f,%d\n", normalize(x, x_min), normalize(y, y_min), l);
            break;
        case 2:
            fprintf(out, "%.6f,%.6f,%d\n", normalize(x, x_max), normalize(y, y_max), l);
            break;
        case 3:
            fprintf(out, "%.6f,%.6f,%d\n", normalize(x, x_min), normalize(y, y_max), l);
            break;
        case 4:
            fprintf(out, "%.6f,%.6f,%d\n", normalize(x, x_max), normalize(y, y_min), l);
            break;
        default:
            break;
        }
    }
    
    fclose(out);
    
    fclose(fp);
    
    return 0;
}