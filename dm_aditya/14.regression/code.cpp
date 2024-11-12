#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <numeric> // For accumulate
#include <cmath>   // For pow

using namespace std;

// Function to calculate mean
double mean(const vector<double>& v) {
    return accumulate(v.begin(), v.end(), 0.0) / v.size();
}

// Function to perform linear regression
void linearRegression(const vector<double>& X, const vector<double>& Y, double& b0, double& b1) {
    double meanX = mean(X);
    double meanY = mean(Y);

    double numerator = 0.0;
    double denominator = 0.0;
    for (size_t i = 0; i < X.size(); i++) {
        numerator += (X[i] - meanX) * (Y[i] - meanY);
        denominator += pow(X[i] - meanX, 2);
    }
    b1 = numerator / denominator;
    b0 = meanY - (b1 * meanX);
}

// Function to read data from CSV file
bool readCSV(const string& filename, vector<double>& X, vector<double>& Y) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return false;
    }

    string line;
    getline(file, line); // Skip header line

    while (getline(file, line)) {
        stringstream ss(line);
        string x_str, y_str;
        
        if (getline(ss, x_str, ',') && getline(ss, y_str)) {
            X.push_back(stod(x_str));
            Y.push_back(stod(y_str));
        }
    }

    file.close();
    return true;
}

int main() {
    vector<double> X, Y;

    // Read data from CSV file
    if (!readCSV("input.csv", X, Y)) {
        return 1;
    }

    double b0, b1;
    linearRegression(X, Y, b0, b1);

    cout << "The linear regression model is: Y = " << b0 << " + " << b1 << " * X" << endl;

    // Example prediction
    double testX = 6.0;
    double predictedY = b0 + b1 * testX;
    cout << "For X = " << testX << ", predicted Y = " << predictedY << endl;

    return 0;
}
