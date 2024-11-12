#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>

using namespace std;

double calculateCorrelation(const vector<double>& X, const vector<double>& Y) {
    if (X.size() != Y.size()) {
        cerr << "Error: Mismatched vector sizes." << endl;
        return -1;
    }
    
    int n = X.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0, sumY2 = 0;
    
    for (int i = 0; i < n; ++i) {
        sumX += X[i];
        sumY += Y[i];
        sumXY += X[i] * Y[i];
        sumX2 += X[i] * X[i];
        sumY2 += Y[i] * Y[i];
    }
    
    double numerator = n * sumXY - sumX * sumY;
    double denominator = sqrt((n * sumX2 - sumX * sumX) * (n * sumY2 - sumY * sumY));
    
    if (denominator == 0) {
        cerr << "Error: Denominator is zero, cannot calculate correlation." << endl;
        return 0;
    }
    
    return numerator / denominator;
}

bool readCSV(const string& filename, vector<double>& column1, vector<double>& column2) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return false;
    }
    
    string line, subject, value1, value2;
    
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, subject, ','); 
        getline(ss, value1, ',');  
        getline(ss, value2, ','); 
        
        try {
            column1.push_back(stod(value1)); 
            column2.push_back(stod(value2)); 
        } catch (invalid_argument&) {
            cerr << "Error: Invalid data format in CSV." << endl;
            return false;
        }
    }
    
    file.close();
    return true;
}

int main() {
    vector<double> studyHours, examScores;
    string filename = "data.csv"; 
    if (!readCSV(filename, studyHours, examScores)) {
        return 1; 
    }

    double correlation = calculateCorrelation(studyHours, examScores);
    
    if (correlation != -1) {
        cout << "The Pearson correlation coefficient is: " << correlation << endl;
    }

    return 0;
}
