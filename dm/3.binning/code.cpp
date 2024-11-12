#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

vector<double> readDataFromCSV(const string& filename) {
    vector<double> data;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Could not open the file: " << filename <<endl;
        return data;
    }

    std::string line;
    while (getline(file, line)) {
        stringstream lineStream(line);
        string cell;
        
        while (std::getline(lineStream, cell, ',')) {
            try {
                double value = std::stod(cell);  
                data.push_back(value);
            } catch (const invalid_argument& e) {
                cerr << "Invalid data encountered: " << cell <<endl;
            }
        }
    }

    file.close();
    return data;
}

// Function to perform binning and output results to a file, including which data points are in each bin
void performBinning(const vector<double>& data, int numBins, const string& outputFilename) {
    if (data.empty()) {
        std::cerr << "No data available to perform binning." << std::endl;
        return;
    }

    // Open output file for writing
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        cerr << "Could not open the output file: " << outputFilename << endl;
        return;
    }

    // Find the minimum and maximum values in the data
    double minVal = *min_element(data.begin(), data.end());
    double maxVal = *max_element(data.begin(), data.end());
    
    // Calculate the bin width
    double binWidth = (maxVal - minVal) / numBins;
    
    // Create bins: vector of vectors to store the data points in each bin
    vector<vector<double>> bins(numBins);
    
    // Assign data points to bins
    for (double value : data) {
        int binIndex = min(static_cast<int>((value - minVal) / binWidth), numBins - 1);
        bins[binIndex].push_back(value);
    }
    
    // Output the binning result to both the console and the output file
    std::cout << "Binning results:\n";
    outputFile << "Binning results:\n";
    for (int i = 0; i < numBins; ++i) {
        double binStart = minVal + i * binWidth;
        double binEnd = binStart + binWidth;
        cout << "Bin " << i + 1 << " (" << binStart << " to " << binEnd << "): " 
                  << bins[i].size() << " items\n";
        outputFile << "Bin " << i + 1 << " (" << binStart << " to " << binEnd << "): " 
                   << bins[i].size() << " items\n";

        // Output the data points in each bin
        cout << "Data in this bin: ";
        outputFile << "Data in this bin: ";
        for (double value : bins[i]) {
            cout << value << " ";
            outputFile << value << " ";
        }
        cout << "\n";
        outputFile << "\n";
    }

    // Close the output file
    outputFile.close();
    cout << "Binning results have been saved to " << outputFilename <<endl;
}

int main() {
    // File containing the CSV data (one column of numbers)
    std::string filename = "data.csv";

    // Output file to store the results
    std::string outputFilename = "output.csv";

    // Read data from CSV file
    std::vector<double> data = readDataFromCSV(filename);

    // Number of bins
    int numBins = 5;
    
    // Perform binning and write the results to the output file
    performBinning(data, numBins, outputFilename);
    
    return 0;
}
