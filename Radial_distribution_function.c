// This code include calculation of Radial distribution funciton using gromacs gro file.
#include <stdio.h>
#include <math.h>
#include <string.h>

#define NP 108   // NUMBER OF ATOMS       
#define BL 1.71     // BOX LENGTH   
#define Rho 21.59      // NUMBER DENSITY(N/V)
#define nhist 500       // NUMBER OF HISTOGRAMS
#define NFRAMES 1000   // NUMBERS OF FRAMES

int main() {
    float positions[NP][3]; 
    char atom[6];           
    char atomtype[6];       
    int atomnumber;         
    int ngr = 0;            

    
    FILE *file = fopen("rdf10.gro", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

   
    double g[nhist] = {0};

    
    double delg = BL / (2.0 * nhist);
    double box = BL;
    double r, xr, yr, zr;

    
    char line[1000]; // Reading of GRO File
    while (fgets(line, sizeof(line), file) != NULL) {
        
        if (fgets(line, sizeof(line), file) == NULL) break; 

       
        for (int i = 0; i < NP; i++) {
            if (fgets(line, sizeof(line), file) != NULL) {
                sscanf(line, "%5s%5s%5i%f%f%f", atom, atomtype, &atomnumber, &positions[i][0], &positions[i][1], &positions[i][2]);
            } else {
                break;
            }
        }

        
        if (fgets(line, sizeof(line), file) == NULL) break;
        
       
        for (int i = 0; i < NP - 1; i++) {
            for (int j = i + 1; j < NP; j++) {
                xr = positions[i][0] - positions[j][0];
                yr = positions[i][1] - positions[j][1];
                zr = positions[i][2] - positions[j][2];

                // APPLYING PBC
                xr = xr - box * round(xr / box);
                yr = yr - box * round(yr / box);
                zr = zr - box * round(zr / box);

                r = sqrt(xr * xr + yr * yr + zr * zr);

                if (r < box / 2.0) {
                    int ig = (int)(r / delg); 
                    g[ig] += 2.0; 
                }
            }
        }

        ngr++; 
    }

    fclose(file);

    
    FILE *output_file = fopen("rdf_output.txt", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error opening output file.\n");
        return 1;
    }
// CACULATION OF RDF
    for (int i = 0; i < nhist; i++) {
        r = delg * (i + 0.5);
        double vb = (4.0 / 3.0) * M_PI * (pow((i + 1) * delg, 3) - pow(i * delg, 3));
        double nid = Rho * vb;
        g[i] = g[i] / (ngr * NP * nid); 
        fprintf(output_file, "%f %f\n", r, g[i]);
    }

    fclose(output_file);

    return 0;
}
