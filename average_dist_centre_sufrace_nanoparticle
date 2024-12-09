#include <stdio.h>
#include <math.h>
#include <string.h>
#define NP 559   
#define NFRAMES 4001   
#define NANO 13        
#define SURFACE 42     
#define box 9.91       
int main() {
    float positions[NP][3];  
    char atom[10];          
    char atomtype[5];       
    int atomnumber;          
    int ngr = 0;             
    float xr, yr, zr, r;     
   
    FILE *file = fopen("frames.gro", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }
    
    FILE *outFiles[NANO];
    for (int i = 0; i < NANO; i++) {
        char filename[20];
        sprintf(filename, "dist-%d.txt", i + 1);
        outFiles[i] = fopen(filename, "w");
        if (outFiles[i] == NULL) {
            fprintf(stderr, "Error opening output file %s.\n", filename);
            return 1;
        }
    }
    char line[1000]; 
    while (ngr < NFRAMES && fgets(line, sizeof(line), file) != NULL) {
        
        if (fgets(line, sizeof(line), file) == NULL) break;
        
        for (int i = 0; i < NP; i++) {
            if (fgets(line, sizeof(line), file) != NULL) {
                sscanf(line, "%s %s %d %f %f %f", atom, atomtype, &atomnumber,
                       &positions[i][0], &positions[i][1], &positions[i][2]);
            } else {
                break;
            }
        }
      
        if (fgets(line, sizeof(line), file) == NULL) break;
        
        for (int np_idx = 0; np_idx < NANO; np_idx++) {
            float avg_dist = 0.0;
            
            int central_atom_idx = np_idx * 43;
            float central_x = positions[central_atom_idx][0];
            float central_y = positions[central_atom_idx][1];
            float central_z = positions[central_atom_idx][2];
            
            for (int i = 1; i <= SURFACE; i++) {
                int surface_atom_idx = central_atom_idx + i;
                float dx = positions[central_atom_idx][0] - positions[surface_atom_idx][0];
                float dy = positions[central_atom_idx][1] - positions[surface_atom_idx][1];
                float dz = positions[central_atom_idx][2] - positions[surface_atom_idx][2];
                
                dx -= box * round(dx / box);
                dy -= box * round(dy / box);
                dz -= box * round(dz / box);
               
                r = sqrt(dx * dx + dy * dy + dz * dz);
                avg_dist += r;
            }
            
            avg_dist /= SURFACE;
           
            fprintf(outFiles[np_idx], "%f\n", avg_dist);
        }
        ngr++;  
    }
    
    for (int i = 0; i < NANO; i++) {
        fclose(outFiles[i]);
    }
    fclose(file);
    return 0;
}
