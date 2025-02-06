#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>

// Code for finding effective radius of polymer-coated nanoparticle 
#define NP 123
#define POL_beads 10
#define NFRAMES 5001
#define box 8.05619

using namespace std;

int main() {
    vector<vector<float>> positions(NP, vector<float>(3));  // Stores the positions of the particles
    string line;
    int ngr = 0;

    ifstream file("frames.gro");
    if (!file.is_open()) {
        cerr << "error opening the file\n";
        return 1;
    }

    // Open output file before the loop to avoid overwriting it every iteration
    ofstream outputfile("effective_radius.txt");
    if (!outputfile) {
        cerr << "error in opening the file: " << "effective_radius.txt" << endl;
        return 1;
    }

    while (getline(file, line)) {
        if (!getline(file, line)) break; // Skip the next line (time step or box size)

        for (int i = 0; i < NP; i++) {
            if (!getline(file, line)) break;  // Get positions of atoms for each frame
            stringstream ss(line);
            string atom, atomtype;
            int atomnumber;
            ss >> atom >> atomtype >> atomnumber >> positions[i][0] >> positions[i][1] >> positions[i][2];
        }

        if (!getline(file, line)) break;  // Skip the last line in the frame

        // Define index of nanoparticle and polymer beads
        int Nindex[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 37, 38, 59, 80, 101, 122};
        int Pindex[] = {39, 40, 41, 42, 43, 44, 45, 46, 47, 48,49, 50, 51, 52, 53,54, 55, 56, 57, 58,60, 61, 62, 63, 64, 65, 67, 68, 69,70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121};

        float Xsum = 0, Ysum = 0, Zsum = 0;

        // Calculation of center of mass for central bead of nanoparticle (mass = 240)
        float Xcentral = 246 * positions[0][0];
        float Ycentral = 246 * positions[0][1];
        float Zcentral = 246 * positions[0][2];

        // Calculation of center of mass for other beads of nanoparticle (each bead mass = 25)
        for (int i = 1; i < sizeof(Nindex) / sizeof(Nindex[0]); i++) {
            int cmIdx = Nindex[i];
            Xsum += 25 * positions[cmIdx][0];
            Ysum += 25 * positions[cmIdx][1];
            Zsum += 25 * positions[cmIdx][2];
        }

        Xsum /= (39 * 25);  // Normalize by total mass
        Ysum /= (39 * 25);
        Zsum /= (39 * 25);

        // Final center of mass coordinate of the nanoparticle
        float Xf = (Xsum + Xcentral) / 246;
        float Yf = (Ysum + Ycentral) / 246;
        float Zf = (Zsum + Zcentral) / 246;

        float rsum = 0;  // Missing semicolon fixed here

        // Loop through each polymer bead index (Pindex)
        for (int p = 0; p < sizeof(Pindex) / sizeof(Pindex[0]); p++) {
            int pIdx = Pindex[p];

            // Calculate relative distances
            float xr = Xf - positions[pIdx][0];
            float yr = Yf - positions[pIdx][1];
            float zr = Zf - positions[pIdx][2];

            // Apply periodic boundary conditions
            xr -= box * round(xr / box);
            yr -= box * round(yr / box);
            zr -= box * round(zr / box);

            // Calculate the distance between polymer bead and nanoparticle
            float r = sqrt(xr * xr + yr * yr + zr * zr);
            rsum += r;  // Accumulate the distances
        }

        // Divide by the number of polymer beads (size of Pindex array)
        rsum /= (sizeof(Pindex) / sizeof(Pindex[0]));  // Correctly divide by the number of polymer beads
        outputfile << rsum << endl;  // Write the average radius to the file

        ngr++;  // Increment number of frames processed
    }

    file.close();
    outputfile.close();  // Close the output file after writing

    return 0;
}

~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
~   
