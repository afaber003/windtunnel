#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
using namespace std;

    // Function Headers //
void readData(const string &, vector<double> &, vector<double> &);
double interpolation(double, const vector<double> &, const vector<double> &);
bool isOrdered(const vector<double> &);
void reorder(vector<double> &, vector<double> &);

int main(int argc, char *argv[]){
    
    string windTunnelFile = argv[1];        // Variable setup
    vector<double> flightAngleVec;
    vector<double> liftCoefficientVec;   
    double userTestAngle;
    char wannastay = 'y';
    double ans;

    readData(windTunnelFile, flightAngleVec, liftCoefficientVec);                                   // Reading data
    while (!isOrdered(flightAngleVec)){                                                             // Reordering data sets
        reorder(flightAngleVec, liftCoefficientVec);
    }


    /* USER INPUT STARTS HERE */

    while (wannastay == 'y'){
        cout << "What angle would you like to approximate?: ";
        cin >> userTestAngle;
        cout << endl;

        if (userTestAngle <= flightAngleVec[flightAngleVec.size() - 1] and userTestAngle >= flightAngleVec[0]){         // Calling Interpolation
            ans = interpolation(userTestAngle, flightAngleVec, liftCoefficientVec);
        } else {cout << "That does input does not fit in the data scope, please try another" << endl; continue;}

        cout << "The estimation for " << userTestAngle << " degrees is " << ans << "." << endl;                         // Answer and continue?
        cout << "Would you like to enter a new flight path angle? (y/n) ";
        cin >> wannastay; cout << endl << endl;
    }

    return 0;
}

void readData(const string &windTunnelFile, vector<double> &flightAngleVec, vector<double> &liftCoefficientVec){
    double temp;
    ifstream inputFile;
    inputFile.open(windTunnelFile);
    
    if (inputFile.is_open() == false){
        cout << "Error opening " << windTunnelFile << endl;
        exit(1);
    }
    while (inputFile >> temp){
        cout << temp << endl;
        flightAngleVec.push_back(temp);
        inputFile >> temp;
        cout << temp << endl;
        liftCoefficientVec.push_back(temp);
    }
    inputFile.close();
}

double interpolation(double userTestAngle, const vector<double> &flightAngleVec, const vector<double> &liftCoefficientVec){
    double total;
    int lowerBounds = -1;
    int upperBounds = -1; 
    int counter = flightAngleVec.size() - 1; // max size

    for (unsigned int i = 0; i < flightAngleVec.size(); i++){        // checking if test value is already known
        if (userTestAngle == flightAngleVec[i]){
            return liftCoefficientVec[i];
        } 
    }

    while(lowerBounds == -1){                               // set lower bounds
        if (userTestAngle > flightAngleVec[counter]){
            lowerBounds = counter;
        } else {counter--;}
    }
    counter = 0; // reset counter to max size
    while(upperBounds == -1){                               // set upper bounds
        if (userTestAngle < flightAngleVec[counter]){
            upperBounds = counter;
        } else {counter++;}
    }
                                                            // Large total equation on line below; 2x checked
    total = liftCoefficientVec[lowerBounds] + ((userTestAngle - flightAngleVec[lowerBounds]) / (flightAngleVec[upperBounds] - flightAngleVec[lowerBounds])) * (liftCoefficientVec[upperBounds] - liftCoefficientVec[lowerBounds]);

    return total;
}

bool isOrdered(const vector<double> &v){           // vector v is ambiguous test vector. will be called only on angle vector
    for (unsigned int i = 0; i < v.size() - 1; i++){
        if (v[i] > v[i+1]){
            return false;
        }
    }
    return true;
}

void reorder(vector<double> &v, vector<double> &alt){        // called to reorder v and keep parallelism with alt
    unsigned int counter = 0;
    double temp;
    while (counter != v.size() - 1){
        for (unsigned int i = 0; i < v.size() - 1; i++){
            if (v[i] > v[i+1]){                 // if fails, v[i] and v[i+1] swaps places
                temp = v[i];
                v[i] = v[i+1];
                v[i+1] = temp;

                temp = alt[i];                  // move secondary values to preserve overall data integrity
                alt[i] = alt[i+1];
                alt[i+1] = temp;

                counter = 0;
                break;
            }
            counter++;
        }
    }
}