#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>

#define NP 123
#define POL_beads 10
#define NFRAMES 5001
#define box 8.05619

using namespace std;

int main() {
    vector<vector<float>> positions(NP, vector<float>(3));
    string atom, atomtype;
    int atomnumber;
    int ngr = 0;

    ifstream file("frames.gro");
    if (!file.is_open()) {
        cerr << "error opening the file \n";
        return 1;
    }

    string line;
    // Read the first header line
    if (!getline(file, line)) {
        cerr << "error reading file header\n";
        return 1;
    }

    // Define index of nanoparticle and polymer beads
    vector<int> Nindex = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 37, 38, 59, 80, 101, 122};
    vector<int> Pindex = {39, 40, 41, 42, 43, 44, 45, 46, 47, 48,49, 50, 51, 52, 53,54, 55, 56, 57, 58,60, 61, 62, 63, 64, 65, 67, 68, 69,70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121};

    while (getline(file, line)) {
        // Read the frame header
        if (!getline(file, line)) break;

        // Read the positions for all particles in this frame
        for (int i = 0; i < NP; i++) {
            if (!getline(file, line)) break;
            stringstream ss(line);
            ss >> atom >> atomtype >> atomnumber >> positions[i][0] >> positions[i][1] >> positions[i][2];
        }

        // Skip the box size line (this may change based on your data format)
        if (!getline(file, line)) break;

        // Loop through each polymer bead index
        for (int p = 0; p < Pindex.size(); p++) {
            int pIdx = Pindex[p];  // The current polymer bead index

            // Open a separate output file for each polymer bead (only once)
            string filename = "data_P" + to_string(pIdx) + ".txt";
            ofstream outputfile(filename, ios::app); // Use append mode to avoid overwriting previous data

            if (!outputfile) {
                cerr << "error in opening the file: " << filename << endl;
                return 1;
            }

            // Calculate distances between Pindex bead and each nanoparticle
            for (int j = 0; j < Nindex.size(); j++) {
                int nIdx = Nindex[j];  // The current nanoparticle index

                float xr = positions[pIdx][0] - positions[nIdx][0];
                float yr = positions[pIdx][1] - positions[nIdx][1];
                float zr = positions[pIdx][2] - positions[nIdx][2];

                // Apply periodic boundary conditions more accurately
                xr = xr - box * round(xr / box);  // Ensure the shift is correctly applied
                yr = yr - box * round(yr / box);
                zr = zr - box * round(zr / box);

                // Calculate the distance and append to the output file
                float r = sqrt(xr * xr + yr * yr + zr * zr);
                outputfile << r << endl;  // Write the distance to the file
            }

            outputfile.close();  // Close the file after writing
        }

        ngr++;
    }

    file.close();
    return 0;
}

