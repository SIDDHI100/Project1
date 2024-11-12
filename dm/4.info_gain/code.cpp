#include <bits/stdc++.h>
using namespace std;

int main()
{
    ifstream file("exp3_input.csv"); // Change this to the correct CSV file name
    ofstream outputFile("output.csv");
    string line, word;
    string outlook, temperature, humidity, windy, playGame;
    map<string, int> parent; // For counting Play/NoPlay instances
    map<string, map<string, int>> child; // For child attribute counts based on Play/NoPlay

    if (!file.is_open())
    {
        perror("Error in opening input file: ");
        return -1;
    }

    int i = 0;
    string childName;
    int choice = 1; // We’ll use the first column (Outlook) by default

    while (getline(file, line))
    {
        stringstream str(line);
        
        // Read data columns
        getline(str, outlook, ',');
        getline(str, temperature, ',');
        getline(str, humidity, ',');
        getline(str, windy, ',');
        getline(str, playGame, ','); // Class label: "Play" or "NoPlay"

        if (i == 0) // Skip header row
        {
            i++;
            cout << "Enter Column Number (1-Outlook, 2-Temperature, 3-Humidity, 4-Windy): ";
            cin >> choice;
            outputFile << "Selected Column: " << choice << "\n";  // Store in file
            continue;
        }

        // Determine the child attribute based on user choice
        switch (choice)
        {
        case 1:
            childName = outlook;
            break;
        case 2:
            childName = temperature;
            break;
        case 3:
            childName = humidity;
            break;
        case 4:
            childName = windy;
            break;
        default:
            childName = outlook;
            break;
        }

        // Count occurrences for Play/NoPlay
        parent[playGame]++;
        child[childName][playGame]++;
    }

    // Calculate Parent Entropy
    double pos = parent["Play"], neg = parent["NoPlay"];
    double total = pos + neg;

    double parent_entropy = 0;
    if (total > 0) {
        parent_entropy = -((pos / total) * log2(pos / total) +
                           (neg / total) * log2(neg / total));
    }
    outputFile << "Parent Entropy: " << parent_entropy << "\n";

    // Calculate Child Entropy
    double child_entropy = 0;
    for (auto p : child)
    {
        string val = p.first;
        double pR = child[val]["Play"], nR = child[val]["NoPlay"];
        double tR = pR + nR;
        if (tR > 0) {
            child_entropy += -((tR / total) * 
                               ((pR / tR) * log2(pR / tR) + (nR / tR) * log2(nR / tR)));
        }
    }
    outputFile << "Child Entropy * Their Proportion: " << child_entropy << "\n";
    outputFile << "Information Gain: " << parent_entropy - child_entropy << "\n";

    // Close files
    file.close();
    outputFile.close();

    cout << "Results written to output.csv" << endl;
    return 0;
}
