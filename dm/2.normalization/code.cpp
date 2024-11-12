#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;

void minMaxNormalization(const vector<double>& data, vector<double>& normalized, int minI, int maxI) {
    double min = *min_element(data.begin(), data.end());
    double max = *max_element(data.begin(), data.end());

    for (double value : data) {
        normalized.push_back((((value - min) / (max - min)) * (maxI - minI)) + minI);
    }
}

void zScoreNormalization(const vector<double>& data, vector<double>& normalized) {
    double mean = 0.0;
    double stdDev = 0.0;

    // Calculate mean
    for (double value : data) {
        mean += value;
    }
    mean /= static_cast<double>(data.size());

    // Calculate standard deviation
    for (double value : data) {
        stdDev += (value - mean) * (value - mean);
    }
    stdDev = std::sqrt(stdDev / static_cast<double>(data.size()));

    // Normalize data
    for (double value : data) {
        normalized.push_back((value - mean) / stdDev);
    }
}

// Function to read student names and marks from CSV file
vector<pair<string, double>> readCSV(const string& filename) {
    vector<pair<string, double>> data;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error opening input file!" << endl;
        return data;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        double mark;
        if (getline(ss, name, ',') && ss >> mark) {
            data.emplace_back(name, mark);
        }
    }

    return data;
}

// Function to write normalized data to output file
void writeToFile(const string& filename, const vector<pair<string, double>>& minMaxNormalized, const vector<pair<string, double>>& zScoreNormalized) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error opening output file!" << endl;
        return;
    }

    outFile << "Min-Max Normalized Data:\n";
    for (const auto& pair : minMaxNormalized) {
        outFile << pair.first << ": " << pair.second << "\n";
    }
    outFile << "\n";

    outFile << "Z-Score Normalized Data:\n";
    for (const auto& pair : zScoreNormalized) {
        outFile << pair.first << ": " << pair.second << "\n";
    }
    outFile << "\n";

    outFile.close();
}

int main() {
    string inputFilename = "data.csv";  // Input CSV file name
    string outputFilename = "normalized_output.csv"; // Output file name

    vector<pair<string, double>> data = readCSV(inputFilename);

    vector<double> marks;
    for (const auto& pair : data) {  // Use pair.first and pair.second instead of structured bindings
        marks.push_back(pair.second);
    }

    vector<double> minMaxNormalizedMarks;
    vector<double> zScoreNormalizedMarks;
    
    cout << "Enter new minimum and maximum values: ";
    double minI, maxI;
    cin >> minI >> maxI;

    // Normalize the marks
    minMaxNormalization(marks, minMaxNormalizedMarks, minI, maxI);
    zScoreNormalization(marks, zScoreNormalizedMarks);

    // Create vectors with names and normalized values
    vector<pair<string, double>> minMaxNormalized, zScoreNormalized;
    for (size_t i = 0; i < data.size(); ++i) {
        minMaxNormalized.emplace_back(data[i].first, minMaxNormalizedMarks[i]);
        zScoreNormalized.emplace_back(data[i].first, zScoreNormalizedMarks[i]);
    }

    // Write results to output file
    writeToFile(outputFilename, minMaxNormalized, zScoreNormalized);

    cout << "Normalization results have been written to " << outputFilename << endl;

    return 0;
}
