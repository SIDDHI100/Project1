#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip> // for std::setprecision

using namespace std;

// Function to read CSV file and store data
vector<vector<string>> read_csv(const string &filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return data; // Return empty data on error
    }

    string line;
    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        string value;
        while (getline(ss, value, ',')) {
            row.push_back(value);
        }
        data.push_back(row);
    }
    return data;
}

// Function to write data to a CSV file
void write_csv(const string &filename, const vector<vector<string>> &data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return; // Exit if file cannot be opened
    }

    for (const auto &row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i != row.size() - 1) file << ",";
        }
        file << "\n";
    }
}

// Function to find correlation between two entities
double find_correlation(const vector<vector<string>> &data, int tid1, int tid2) {
    int tid1_count = 0;
    int tid2_count = 0;
    int total_common_count = 0;

    // Loop through columns 1 to 7 (adjusted for 0-based indexing)
    for (int j = 1; j <= 7; ++j) {
        if (data[tid1][j] == "Y") tid1_count++;
        if (data[tid2][j] == "Y") tid2_count++;
        if (data[tid1][j] == "Y" && data[tid2][j] == "Y") total_common_count++;
    }

    if (tid1_count == 0 || tid2_count == 0) return 0.0;

    return static_cast<double>(total_common_count) / (tid1_count * tid2_count);
}

int main() {
    // Read input data from CSV file
    vector<vector<string>> sheet = read_csv("Correlation_Input.csv");

    if (sheet.size() < 2) {
        cerr << "Not enough data to compute correlation." << endl;
        return 1; // Exit if there's not enough data
    }

    int n = sheet.size() - 1; // Number of rows (excluding header)

    // Store data for output
    vector<vector<string>> output_data;
    output_data.push_back({"Item 1 with tid", "Item 2 with tid", "Correlation coefficient", "Type of correlation"});

    // Iterate through pairs of entities
    for (int i = 1; i <= n; ++i) { // Start from 1 since 0 is header
        for (int j = i + 1; j <= n; ++j) {
            double ans = find_correlation(sheet, i, j);
            string verdict;

            if (ans == 0)
                verdict = "No relationship between entities";
            else if (ans < 0)
                verdict = "Negative correlation";
            else if (ans > 0)
                verdict = "Positive correlation";
            else
                verdict = "Not defined";

            cout << "Correlation ratio " << i << " & " << j << " = " 
                 << fixed << setprecision(4) << ans << " " << verdict << endl;

            // Prepare row for output
            vector<string> row = {to_string(i), to_string(j), to_string(ans), verdict};
            output_data.push_back(row);
        }
    }

    // Write output data to a CSV file
    write_csv("Correlation_output.csv", output_data);

    return 0;
}
