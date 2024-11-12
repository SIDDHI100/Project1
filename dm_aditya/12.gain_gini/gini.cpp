#include<bits/stdc++.h>
using namespace std;

// Function to calculate entropy
double calculateEntropy(double yes, double no) {
    double total = yes + no;
    if (total == 0) return 0; // To avoid division by zero
    double p_yes = yes / total;
    double p_no = no / total;

    double entropy = 0;
    if (p_yes > 0) entropy -= p_yes * log2(p_yes);
    if (p_no > 0) entropy -= p_no * log2(p_no);

    return entropy;
}

// Function to calculate Gini Index
double calculateGini(double yes, double no) {
    double total = yes + no;
    if (total == 0) return 0; // To avoid division by zero
    double p_yes = yes / total;
    double p_no = no / total;

    double gini = 1 - (p_yes * p_yes + p_no * p_no);
    return gini;
}

// Function to read CSV file and parse the data
void readCSV(const string& filename, vector<vector<string>>& data) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        vector<string> row;
        while (getline(ss, value, ',')) {
            row.push_back(value);
        }
        data.push_back(row);
    }
    file.close();
}

// Function to write result to CSV file
void writeCSV(const string& filename, const vector<string>& logData) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    for (const string& line : logData) {
        file << line << "\n";
    }

    file.close();
}

// Function to check if a string is numeric
bool isNumeric(const string& str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

// Function to split and calculate for numerical data
void calculateForNumerical(vector<vector<string>>& data, int featureCol, int targetCol, const map<string, double>& targetCount, vector<string>& logData, double parentEntropy, int totalSamples, double& bestFinalGini) {
    set<double> uniqueValues;
    for (int i = 1; i < data.size(); i++) {
        uniqueValues.insert(stod(data[i][featureCol]));
    }

    double bestInfoGain = -1;
    string bestSplit;
    for (double splitPoint : uniqueValues) {
        double leftYes = 0, leftNo = 0, rightYes = 0, rightNo = 0;

        for (int i = 1; i < data.size(); i++) {
            double featureValue = stod(data[i][featureCol]);
            string targetValue = data[i][targetCol];

            if (featureValue <= splitPoint) {
                if (targetValue == "Yes") leftYes++;
                else leftNo++;
            } else {
                if (targetValue == "Yes") rightYes++;
                else rightNo++;
            }
        }

        double leftTotal = leftYes + leftNo;
        double rightTotal = rightYes + rightNo;
        double weightedEntropy = ((leftTotal / totalSamples) * calculateEntropy(leftYes, leftNo)) +
                                 ((rightTotal / totalSamples) * calculateEntropy(rightYes, rightNo));
        double weightedGini = ((leftTotal / totalSamples) * calculateGini(leftYes, leftNo)) +
                              ((rightTotal / totalSamples) * calculateGini(rightYes, rightNo));

        double infoGain = parentEntropy - weightedEntropy;

        string logEntry = "Split Point: " + to_string(splitPoint) + ", Weighted Entropy: " + to_string(weightedEntropy) +
                          ", Gini Index: " + to_string(weightedGini) + ", Information Gain: " + to_string(infoGain);
        logData.push_back(logEntry);

        if (infoGain > bestInfoGain) {
            bestInfoGain = infoGain;
            bestSplit = to_string(splitPoint);
            bestFinalGini = weightedGini;
        }
    }

    logData.push_back("Best Split Point: " + bestSplit + " with Information Gain: " + to_string(bestInfoGain));
}

// Function to split and calculate for categorical data
void calculateForCategorical(vector<vector<string>>& data, int featureCol, int targetCol, const map<string, double>& targetCount, vector<string>& logData, double parentEntropy, int totalSamples, double& bestFinalGini) {
    map<string, map<string, double>> featureMap; // {feature_value -> {target_value -> count}}
    
    for (int i = 1; i < data.size(); i++) {
        string featureValue = data[i][featureCol];
        string targetValue = data[i][targetCol];

        featureMap[featureValue][targetValue]++;
    }

    double weightedEntropy = 0;
    double weightedGiniIndex = 0;
    for (const auto& featureEntry : featureMap) {
        double featureTotal = 0;
        double featureEntropy = 0;
        double featureGini = 0;

        for (const auto& targetEntry : featureEntry.second) {
            featureTotal += targetEntry.second;
        }

        for (const auto& targetEntry : featureEntry.second) {
            double p = targetEntry.second / featureTotal;
            if (p > 0) featureEntropy -= p * log2(p);
        }

        for (const auto& targetEntry : featureEntry.second) {
            double p = targetEntry.second / featureTotal;
            featureGini += p * p;
        }
        featureGini = 1 - featureGini;

        weightedEntropy += (featureTotal / totalSamples) * featureEntropy;
        weightedGiniIndex += (featureTotal / totalSamples) * featureGini;

        logData.push_back("Feature: " + featureEntry.first + " | Weighted Entropy: " + to_string(featureEntropy) + " | Gini Index: " + to_string(featureGini));
    }

    double infoGain = parentEntropy - weightedEntropy;
    bestFinalGini = weightedGiniIndex;
    logData.push_back("Information Gain for Selected Feature: " + to_string(infoGain));
}

int main() {
    vector<vector<string>> data;

    string inputFile = "input.csv";  
    readCSV(inputFile, data);

    int targetCol, featureCol;
    cout << "Enter the index of the target column (0-indexed): ";
    cin >> targetCol;
    cout << "Enter the index of the feature column (0-indexed): ";
    cin >> featureCol;

    if (data.size() < 2 || targetCol >= data[0].size() || featureCol >= data[0].size()) {
        cerr << "Invalid input or no data found in the CSV file." << endl;
        return 1;
    }

    map<string, double> targetCount;
    int totalSamples = data.size() - 1;

    for (int i = 1; i < data.size(); i++) {
        targetCount[data[i][targetCol]]++;
    }

    double parentEntropy = 0;
    for (const auto& entry : targetCount) {
        double count = entry.second;
        double p = count / totalSamples;
        if (p > 0) parentEntropy -= p * log2(p);
    }

    vector<string> logData;
    logData.push_back("Parent Entropy: " + to_string(parentEntropy));

    double bestFinalGini = 0;

    if (isNumeric(data[1][featureCol])) {
        logData.push_back("Numerical Feature Selected");
        calculateForNumerical(data, featureCol, targetCol, targetCount, logData, parentEntropy, totalSamples, bestFinalGini);
    } else {
        logData.push_back("Categorical Feature Selected");
        calculateForCategorical(data, featureCol, targetCol, targetCount, logData, parentEntropy, totalSamples, bestFinalGini);
    }

    logData.push_back("Final Gini Index after Best Split: " + to_string(bestFinalGini));

    string outputFile = "output.csv";
    writeCSV(outputFile, logData);

    cout << "Results written to " << outputFile << endl;

    return 0;
}