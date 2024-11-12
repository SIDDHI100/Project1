#include<bits/stdc++.h>
using namespace std;

// Function to calculate mean of a vector
double calculateMean(const vector<double> &values)
{
    double sum = 0;
    for (double value : values)
    {
        sum += value;
    }
    return sum / values.size();
}

// Function to calculate standard deviation of a vector
double calculateStdDev(const vector<double> &values, double mean)
{
    double sum = 0;
    for (double value : values)
    {
        sum += pow(value - mean, 2);
    }
    return sqrt(sum / values.size());
}

// Gaussian Probability Density Function for numerical data
double calculateGaussianProbability(double x, double mean, double stdDev)
{
    return (1 / (sqrt(2 * M_PI) * stdDev)) * exp(-pow(x - mean, 2) / (2 * pow(stdDev, 2)));
}

// Function to read CSV file and parse the data
void readCSV(const string &filename, vector<vector<string>> &data)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string value;
        vector<string> row;
        while (getline(ss, value, ','))
        {
            row.push_back(value);
        }
        data.push_back(row);
    }
    file.close();
}

// Function to write output to CSV file
void writeCSV(const string &filename, const vector<string> &logData)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    for (const string &line : logData)
    {
        file << line << "\n";
    }
    file.close();
}

// Function to train Naive Bayes for categorical and numerical data
void trainNaiveBayes(const vector<vector<string>> &data, int targetCol,
                     map<string, map<int, map<string, double>>> &categoricalCounts,
                     map<string, map<int, vector<double>>> &numericalStats,
                     map<string, int> &classCounts)
{

    for (int i = 1; i < data.size(); i++)
    {
        string targetClass = data[i][targetCol];
        classCounts[targetClass]++;

        for (int j = 0; j < data[0].size(); j++)
        {
            if (j == targetCol)
                continue; // Skip the target column

            if (isdigit(data[i][j][0]))
            { // Numerical data
                numericalStats[targetClass][j].push_back(stod(data[i][j]));
            }
            else
            { // Categorical data
                categoricalCounts[targetClass][j][data[i][j]]++;
            }
        }
    }
}

// Function to predict class for a new instance and log all probabilities to CSV
string predictNaiveBayes(const vector<vector<string>> &data, int targetCol,
                         const map<string, map<int, map<string, double>>> &categoricalCounts,
                         const map<string, map<int, vector<double>>> &numericalStats,
                         const map<string, int> &classCounts,
                         const vector<string> &instance, vector<string> &logData)
{

    int totalSamples = data.size() - 1;
    double bestProb = -1;
    string bestClass;

    logData.push_back("Class,Prior Probability,Feature Probabilities,Total Probability");

    for (const auto &classEntry : classCounts)
    {
        string targetClass = classEntry.first;
        double classProb = (double)classEntry.second / totalSamples;
        double initialClassProb = classProb; // Save initial class probability for logging
        string logLine = targetClass + "," + to_string(classProb);

        // Calculate probabilities for each feature
        for (int j = 0; j < instance.size(); j++)
        {
            if (j == targetCol)
                continue; // Skip the target column

            if (isdigit(instance[j][0]))
            { // Numerical feature
                double mean = calculateMean(numericalStats.at(targetClass).at(j));
                double stdDev = calculateStdDev(numericalStats.at(targetClass).at(j), mean);
                double featureProb = calculateGaussianProbability(stod(instance[j]), mean, stdDev);
                classProb *= featureProb;

                logLine += "," + to_string(featureProb); // Log feature probability
            }
            else
            { // Categorical feature
                if (categoricalCounts.at(targetClass).at(j).count(instance[j]) > 0)
                {
                    double featureProb = categoricalCounts.at(targetClass).at(j).at(instance[j]) / classEntry.second;
                    classProb *= featureProb;

                    logLine += "," + to_string(featureProb); // Log feature probability
                }
                else
                {
                    double featureProb = 1.0 / (classEntry.second + categoricalCounts.at(targetClass).at(j).size());
                    classProb *= featureProb;

                    logLine += "," + to_string(featureProb); // Log feature probability
                }
            }
        }

        logLine += "=" + to_string(classProb); // Log final class probability
        logData.push_back(logLine);

        if (classProb > bestProb)
        {
            bestProb = classProb;
            bestClass = targetClass;
        }
    }

    return bestClass;
}

int main()
{
    vector<vector<string>> data;

    string inputFile = "input.csv";
    readCSV(inputFile, data);

    // User provides the index of the target (class) column
    int targetCol;
    cout << "Enter the index of the target column (0-indexed): ";
    cin >> targetCol;

    map<string, map<int, map<string, double>>> categoricalCounts;
    map<string, map<int, vector<double>>> numericalStats;
    map<string, int> classCounts;

    // Train Naive Bayes
    trainNaiveBayes(data, targetCol, categoricalCounts, numericalStats, classCounts);

    // Ask user for a new instance for prediction
    vector<string> newInstance(data[0].size());
    cout << "Enter the values for the new instance: " << endl;
    for (int i = 0; i < data[0].size(); i++)
    {
        if (i == targetCol)
            continue;
        cout << data[0][i] << ": ";
        cin >> newInstance[i];
    }

    // Log data for writing to CSV
    vector<string> logData;

    // Predict the class and log intermediate results
    string predictedClass = predictNaiveBayes(data, targetCol, categoricalCounts, numericalStats, classCounts, newInstance, logData);

    logData.push_back("Predicted Class: " + predictedClass);

    // Write results to output.csv
    string outputFile = "output.csv";
    writeCSV(outputFile, logData);

    cout << "Predicted Class: " << predictedClass << endl;
    cout << "Results and intermediate calculations written to " << outputFile << endl;

    return 0;
}