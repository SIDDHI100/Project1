#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>
#include <algorithm> // For std::min_element and std::max_element

using namespace std;

void minMaxNormalization(const std::vector<double>& data, std::vector<double>& normalized) {
    double min = *min_element(data.begin(), data.end());
    double max = *max_element(data.begin(), data.end());

    for (double value : data) {
        normalized.push_back((value - min) / (max - min));
    }
}

void zScoreNormalization(const std::vector<double>& data, std::vector<double>& normalized) {
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

std::vector<double> readCSV(const std::string& filename) {
    std::vector<double> data;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        while (std::getline(ss, value, ',')) {
            data.push_back(std::stod(value));
        }
    }

    return data;
}

void writeToFile(const std::string& filename, const std::vector<double>& minMaxNormalized, const std::vector<double>& zScoreNormalized) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file!" << std::endl;
        return;
    }

    outFile << "Min-Max Normalized Data:\n";
    for (double value : minMaxNormalized) {
        outFile << value << " ";
    }
    outFile << "\n\n";

    outFile << "Z-Score Normalized Data:\n";
    for (double value : zScoreNormalized) {
        outFile << value << " ";
    }
    outFile << "\n";

    outFile.close();
}

int main() {
    std::string inputFilename = "data.csv";  // Input CSV file name
    std::string outputFilename = "normalized_output.txt"; // Output file name

    std::vector<double> data = readCSV(inputFilename);
    
    std::vector<double> minMaxNormalized;
    std::vector<double> zScoreNormalized;

    minMaxNormalization(data, minMaxNormalized);
    zScoreNormalization(data, zScoreNormalized);

    // Write results to output file
    writeToFile(outputFilename, minMaxNormalized, zScoreNormalized);

    std::cout << "Normalization results have been written to " << outputFilename << std::endl;

    return 0;
}
