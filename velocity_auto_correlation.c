#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_ATOMS  864 
#define NUM_FRAMES 800 

void calculate_vacf(float velocities[NUM_FRAMES][NUM_ATOMS][3], float vacf[NUM_FRAMES]);

int main() {
    float velocities[NUM_FRAMES][NUM_ATOMS][3];
    char atom[6];           
    char atomtype[6];       
    int atomnumber;         
    
    FILE *file = fopen("ndx.gro", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    char line[1000];
    int frame = 0;

    
    while (frame < NUM_FRAMES && fgets(line, sizeof(line), file) != NULL) {
        
        fgets(line, sizeof(line), file);
        fgets(line, sizeof(line), file); 
        for (int i = 0; i < NUM_ATOMS; i++) {
            if (fgets(line, sizeof(line), file) != NULL) {
                sscanf(line, "%5s %5s %5i %*f %*f %*f %f %f %f", atom, atomtype, &atomnumber, 
                       &velocities[frame][i][0], &velocities[frame][i][1], &velocities[frame][i][2]);
            } else {
                break;
            }
        }
        frame++;
    }
    fclose(file);

    
    float vacf[NUM_FRAMES] = {0};

   
    calculate_vacf(velocities, vacf);

    
    FILE *output = fopen("vacf.txt", "w");
    if (output == NULL) {
        fprintf(stderr, "Error opening output file.\n");
        return 1;
    }
    for (int t = 0; t < NUM_FRAMES; t++) {
        fprintf(output, "%f %f\n", t * 0.01, vacf[t]);
    }
    fclose(output);

    return 0;
}

void calculate_vacf(float velocities[NUM_FRAMES][NUM_ATOMS][3], float vacf[NUM_FRAMES]) {
    printf("Calculating VACF");

    
    double norm_factor = 0.0;
    for (int i = 0; i < NUM_ATOMS; i++) {
        double vx0 = velocities[0][i][0];
        double vy0 = velocities[0][i][1];
        double vz0 = velocities[0][i][2];
        norm_factor += (vx0 * vx0 + vy0 * vy0 + vz0 * vz0);
    }
    norm_factor /= NUM_ATOMS;

   
    for (int dt = 0; dt < NUM_FRAMES; dt++) {
        double total_vacf = 0.0;
        int count = 0;

        for (int t = 0; t < NUM_FRAMES - dt; t++) {
            double sum_dot_product = 0.0;

            for (int i = 0; i < NUM_ATOMS; i++) {
                
                double vx0 = velocities[t][i][0];
                double vy0 = velocities[t][i][1];
                double vz0 = velocities[t][i][2];
                
                double vxt = velocities[t + dt][i][0];
                double vyt = velocities[t + dt][i][1];
                double vzt = velocities[t + dt][i][2];

                sum_dot_product += (vx0 * vxt + vy0 * vyt + vz0 * vzt);
            }

            total_vacf += sum_dot_product / NUM_ATOMS; 
            count++;
        }

        vacf[dt] = total_vacf / count / norm_factor; 
    }

    
    double initial_vacf = vacf[0];
    for (int dt = 0; dt < NUM_FRAMES; dt++) {
        vacf[dt] /= initial_vacf;
    }
}
