#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_ATOMS 864 
#define NUM_FRAMES 800 
#define TIME_STEP 0.01 

void unfold_positions(float positions[NUM_FRAMES][NUM_ATOMS][3], float box_len[NUM_FRAMES]);
void calculate_msd(float positions[NUM_FRAMES][NUM_ATOMS][3], float box_len[NUM_FRAMES]);

int main() {
    float positions[NUM_FRAMES][NUM_ATOMS][3]; 
    float box_len[NUM_FRAMES]; 
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
        for (int i = 0; i < NUM_ATOMS; i++) {
            if (fgets(line, sizeof(line), file) != NULL) {
                sscanf(line, "%5s %5s %5i %f %f %f", atom, atomtype, &atomnumber, 
                       &positions[frame][i][0], &positions[frame][i][1], &positions[frame][i][2]);
            } else {
                break;
            }
        }
        if (fgets(line, sizeof(line), file) != NULL) {
            sscanf(line, "%f", &box_len[frame]); 
        }
        frame++;
    }

    fclose(file);

    
    unfold_positions(positions, box_len);

    
    calculate_msd(positions, box_len);

    return 0;
}

void unfold_positions(float positions[NUM_FRAMES][NUM_ATOMS][3], float box_len[NUM_FRAMES]) {
    for (int j = 0; j < NUM_ATOMS; j++) {
        for (int i = 0; i < NUM_FRAMES - 1; i++) {
            for (int k = 0; k < 3; k++) {
                
                float delta = positions[i + 1][j][k] - positions[i][j][k];
                delta -= round(delta / box_len[i]) * box_len[i];
                positions[i + 1][j][k] = positions[i][j][k] + delta;
            }
        }
    }
}

void calculate_msd(float positions[NUM_FRAMES][NUM_ATOMS][3], float box_len[NUM_FRAMES]) {
    double msd[NUM_FRAMES] = {0}; 
    int max_tau = NUM_FRAMES ;


    printf("Calculating MSD, sit back and relax\n");
    for (int dt = 1; dt <= max_tau; dt++) {
        double total_msd = 0.0;
        int count = 0;

        for (int t = 0; t <= NUM_FRAMES - dt - 1; t++) {
            double sum_sq_displacement = 0.0;

            for (int i = 0; i < NUM_ATOMS; i++) {
                
                double dx = positions[t + dt][i][0] - positions[t][i][0];
                double dy = positions[t + dt][i][1] - positions[t][i][1];
                double dz = positions[t + dt][i][2] - positions[t][i][2];

                sum_sq_displacement += (dx * dx + dy * dy + dz * dz);
            }

            total_msd += sum_sq_displacement / NUM_ATOMS; 
            count++;
        }

        msd[dt] = total_msd / count; 

        
        printf("dt = %d, MSD = %f\n", dt, msd[dt]);
    }

    
    FILE *output_file = fopen("msd_output1.txt", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        exit(1);
    }

    for (int dt = 1; dt <= max_tau; dt++) {
        fprintf(output_file, "%e %f\n", dt * TIME_STEP , msd[dt]); 
        printf("time (s) = %e, MSD = %f\n", dt * TIME_STEP, msd[dt]);
    }

    fclose(output_file);
    printf("MSD results written to msd_output.txt\n");
}
