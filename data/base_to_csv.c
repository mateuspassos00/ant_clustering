#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() {    

    float x, y;
    int label;
    char line[256];
    
    FILE *fp = fopen("base_15_grupos", "rt"); assert(fp != NULL);
    FILE *fq = fopen("base_15_grupos.csv", "wt"); assert(fq != NULL);
    
    while (fgets(line, sizeof(line), fp)) {        
        for (char *p = line; *p; ++p) if (*p == ',') *p = '.'; // <-- aprender essa tech

        if (sscanf(line, "%f %f %d", &x, &y, &label) == 3) {
            fprintf(fq, "%f,%f,%d\n", x, y,label);
        }
    }
    

    fclose(fp);
    fclose(fq);

    return 0;
}