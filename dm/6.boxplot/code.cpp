#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<vector<double>> readCSV(const string &filePath)
{
    ifstream file(filePath);
    vector<vector<double>> data;
    string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            stringstream ss(line);
            string value;
            vector<double> row;
            while (getline(ss, value, ','))
            {
                row.push_back(stod(value)); // Convert string to double
            }
            data.push_back(row);
        }
        file.close();
    }

    return data;
}

struct Summary
{
    double min;
    double Q1;
    double median;
    double Q3;
    double max;
    double IQR;
};

Summary fiveNumberSummary(const vector<double> &column)
{
    Summary summary;
    vector<double> sortedCol = column;
    sort(sortedCol.begin(), sortedCol.end());
    int n = sortedCol.size();

    summary.min = sortedCol[0];
    summary.max = sortedCol[n - 1];

    // Median
    if (n % 2 == 0)
    {
        summary.median = (sortedCol[n / 2 - 1] + sortedCol[n / 2]) / 2.0;
    }
    else
    {
        summary.median = sortedCol[n / 2];
    }

    // Q1
    vector<double> lowerHalf(sortedCol.begin(), sortedCol.begin() + n / 2);
    int lowerSize = lowerHalf.size();
    if (lowerSize % 2 == 0)
    {
        summary.Q1 = (lowerHalf[lowerSize / 2 - 1] + lowerHalf[lowerSize / 2]) / 2.0;
    }
    else
    {
        summary.Q1 = lowerHalf[lowerSize / 2];
    }

    // Q3
    vector<double> upperHalf(sortedCol.begin() + (n + 1) / 2, sortedCol.end());
    int upperSize = upperHalf.size();
    if (upperSize % 2 == 0)
    {
        summary.Q3 = (upperHalf[upperSize / 2 - 1] + upperHalf[upperSize / 2]) / 2.0;
    }
    else
    {
        summary.Q3 = upperHalf[upperSize / 2];
    }
    summary.IQR = summary.Q3 - summary.Q1;
    return summary;
}

// Function to process CSV and write summaries to an output file
void processCSV(const string &inputFilePath, const string &outputFilePath)
{
    vector<vector<double>> data = readCSV(inputFilePath);

    int rows = data.size();
    int columns = data.empty() ? 0 : data[0].size(); // Check for empty data

    if (rows == 0 || columns == 0)
    {
        cout << "No data found!" << endl;
        return;
    }

    // Transpose the data to process each column
    vector<vector<double>> transposed(columns, vector<double>(rows));
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            transposed[j][i] = data[i][j];
        }
    }

    // Create output file stream
    ofstream outputFile(outputFilePath);
    if (!outputFile.is_open())
    {
        cout << "Error opening output file!" << endl;
        return;
    }

    // Calculate and write summary for each column
    for (int i = 0; i < columns; ++i)
    {
        Summary summary = fiveNumberSummary(transposed[i]);
        outputFile << "Summary for Column " << i + 1 << ":\n";
        outputFile << "Min: " << summary.min << "\n";
        outputFile << "Q1: " << summary.Q1 << "\n";
        outputFile << "Median: " << summary.median << "\n";
        outputFile << "Q3: " << summary.Q3 << "\n";
        outputFile << "Max: " << summary.max << "\n";
        outputFile << "IQR: " << summary.IQR << "\n\n";
    }

    outputFile.close();
}

int main()
{
    string inputFile = "data.csv";       // Input CSV file path
    string outputFile = "summary.csv";    // Output summary file path
    processCSV(inputFile, outputFile);
    return 0;
}
